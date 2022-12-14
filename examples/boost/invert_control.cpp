#include <boost/context/fiber.hpp>
#include <boost/context/stack_context.hpp>
#include <boost/context/fixedsize_stack.hpp>
#include <iostream>
#include <sstream>

namespace ctx=boost::context;
using ctx::stack_context;
using ctx::fixedsize_stack;

int main() {
    /*
    * grammar:
    *   P ---> E '\0'
    *   E ---> T {('+'|'-') T}
    *   T ---> S {('*'|'/') S}
    *   S ---> digit | '(' E ')'
    */
    class Parser{
    char next;
    std::istream& is;
    std::function<void(char)> cb;

    char pull(){
            return std::char_traits<char>::to_char_type(is.get());
    }

    void scan(){
        do{
            next=pull();
        }
        while(isspace(next));
    }

    public:
    Parser(std::istream& is_,std::function<void(char)> cb_) :
        next(), is(is_), cb(cb_)
        {}

    void run() {
        scan();
        E();
    }

    private:
    void E(){
        T();
        while (next=='+'||next=='-'){
            cb(next);
            scan();
            T();
        }
    }

    void T(){
        S();
        while (next=='*'||next=='/'){
            cb(next);
            scan();
            S();
        }
    }

    void S(){
        if (isdigit(next)){
            cb(next);
            scan();
        }
        else if(next=='('){
            cb(next);
            scan();
            E();
            if (next==')'){
                cb(next);
                scan();
            }else{
                throw std::runtime_error("parsing failed");
            }
        }
        else{
            throw std::runtime_error("parsing failed");
        }
    }
    };

    std::istringstream is("1+1");
    // user-code pulls parsed data from parser
    // invert control flow
    char c;
    bool done=false;
    // execute parser in new fiber
    ctx::fiber source{[&is,&c,&done](ctx::fiber&& sink){
        // create parser with callback function
        Parser p(is,
                [&sink,&c](char c_){
                    // resume main fiber
                    c=c_;
                    sink=std::move(sink).resume();
                });
            // start recursive parsing
            p.run();
            // signal termination
            done=true;
            // resume main fiber
            return std::move(sink);
    }};
    source=std::move(source).resume();
    while(!done){
        printf("Parsed: %c\n",c);
        source=std::move(source).resume();
    }

    return 0;
}