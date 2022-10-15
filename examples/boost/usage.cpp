#include <boost/context/fiber.hpp>
#include <iostream>

namespace ctx=boost::context;

int main() {
    int a;
    ctx::fiber source{[&a](ctx::fiber&& sink){
        a=0;
        int b=1;
        for(;;){
            sink=std::move(sink).resume();
            int next=a+b;
            a=b;
            b=next;
        }
        return std::move(sink);
    }};
    for (int j=0;j<10;++j) {
        source=std::move(source).resume();
        std::cout << a << " ";
    }
    return 0;
}