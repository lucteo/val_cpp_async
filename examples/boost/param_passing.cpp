#include <boost/context/fiber.hpp>
#include <iostream>

namespace ctx=boost::context;

int main() {
    int i=1;
    ctx::fiber f1{[&i](ctx::fiber&& f2){
        std::printf("inside f1,i==%d\n",i);
        i+=1;
        return std::move(f2).resume();
    }};
    f1=std::move(f1).resume();
    std::printf("i==%d\n",i);
    return 0;
}