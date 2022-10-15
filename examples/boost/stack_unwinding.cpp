#include <boost/context/fiber.hpp>
#include <boost/context/stack_context.hpp>
#include <boost/context/fixedsize_stack.hpp>
#include <iostream>

namespace ctx=boost::context;
using ctx::fixedsize_stack;
using ctx::stack_context;
using ctx::preallocated;

struct my_control_structure  {
    // captured fiber
    ctx::fiber   f;

    template< typename StackAllocator >
    my_control_structure(void * sp,std::size_t size,stack_context sctx,StackAllocator salloc) :
        // create captured fiber
        f{std::allocator_arg,preallocated(sp,size,sctx),salloc,entry_func} {
    }
    //...
};

int main() {
    // stack-allocator used for (de-)allocating stack
    fixedsize_stack salloc(4048);
    // allocate stack space
    stack_context sctx(salloc.allocate());
    // reserve space for control structure on top of the stack
    void * sp=static_cast<char*>(sctx.sp)-sizeof(my_control_structure);
    std::size_t size=sctx.size-sizeof(my_control_structure);
    // placement new creates control structure on reserved space
    my_control_structure * cs=new(sp)my_control_structure(sp,size,sctx,salloc);
    //...
    // destructing the control structure
    cs->~my_control_structure();
    // ...
    return 0;
}