#include <boost/context/fiber.hpp>
#include <iostream>

namespace ctx=boost::context;

int main() {
    struct my_exception : public std::runtime_error {
        ctx::fiber    f;
        my_exception(ctx::fiber&& f_,std::string const& what) :
            std::runtime_error{ what },
            f{ std::move(f_) } {
        }
    };

    ctx::fiber f{[](ctx::fiber && f) ->ctx::fiber {
        std::cout << "entered" << std::endl;
        try {
            f=std::move(f).resume();
        } catch (my_exception & ex) {
            std::cerr << "my_exception: " << ex.what() << std::endl;
            return std::move(ex.f);
        }
        return {};
    }};
    f=std::move(f).resume();
    f=std::move(f).resume_with([](ctx::fiber && f) ->ctx::fiber {
        throw my_exception(std::move(f),"abc");
        return {};
    });
    return 0;
}