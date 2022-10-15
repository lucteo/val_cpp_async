#include <boost/context/fiber.hpp>
#include <iostream>
#include <thread>
#include <semaphore>
#include <cstdio>

namespace ctx = boost::context;

class thread_exec_context {
    std::thread thread_;
    ctx::fiber cont_fiber_;

public:
    thread_exec_context() = default;
    ~thread_exec_context() {
        // Resume the thread that was stopped when our thread was created
        std::move(cont_fiber_).resume();
        thread_.join(); // not best practice, but it just makes things easier
    }

    // Switches the current execution flow to this thread
    void enter() {
        ctx::fiber switch_fib{[this](ctx::fiber&& exit_point) -> ctx::fiber {
            std::binary_semaphore sem{0};

            ctx::fiber thread_cont;
            thread_ = std::thread{[&exit_point, &thread_cont, &sem] {
                thread_cont = std::move(exit_point).resume();
                // we will exit this point after ~thread_exec_context() is called
                sem.release();
            }};
            // Pause the current thread; we will use the new thread from now on
            // We will unblock it inside ~thread_exec_context()
            sem.acquire();
            return thread_cont;
        }};
        // Start a continuation that will move the execution context on a new thread
        cont_fiber_ = std::move(switch_fib).resume();
        // This is resumed on the new thread
    }
};

int main() {
    {
        thread_exec_context th1;
        thread_exec_context th2;
        thread_exec_context th3;

        std::cout << std::this_thread::get_id() << ": start\n";

        th1.enter();
        std::cout << std::this_thread::get_id() << ": Hello, world! (from first thread)\n";

        th2.enter();
        std::cout << std::this_thread::get_id() << ": Hello, world! (from second thread)\n";

        th3.enter();
        std::cout << std::this_thread::get_id() << ": Hello, world! (from third thread)\n";
    }
    std::cout << std::this_thread::get_id() << ": Done\n";

    return 0;
}