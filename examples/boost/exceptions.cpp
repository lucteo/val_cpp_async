#include <boost/context/fiber.hpp>
#include <iostream>

namespace ctx=boost::context;

int main() {
    int data=0;
    ctx::fiber f1{[&data](ctx::fiber&& f2) {
        std::cout << "f1: entered first time: " << data << std::endl;
        data+=1;
        f2=std::move(f2).resume();
        std::cout << "f1: entered second time: " << data << std::endl;
        data+=1;
        f2=std::move(f2).resume();
        std::cout << "f1: entered third time: " << data << std::endl;
        return std::move(f2);
    }};
    f1=std::move(f1).resume();
    std::cout << "f1: returned first time: " << data << std::endl;
    data+=1;
    f1=std::move(f1).resume();
    std::cout << "f1: returned second time: " << data << std::endl;
    data+=1;
    f1=std::move(f1).resume_with([&data](ctx::fiber&& f2){
        std::cout << "f2: entered: " << data << std::endl;
        data=-1;
        return std::move(f2);
    });
    std::cout << "f1: returned third time" << std::endl;
    return 0;
}