cmake_minimum_required(VERSION 3.17.0)

# Define the project
project(val_async_cpp-dev LANGUAGES CXX)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else ()
    message(ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

# Set the version of the project
project(val_async_cpp-dev VERSION "${CONAN_PACKAGE_VERSION}")

# Print CMake configuration
message(STATUS "System           : ${CMAKE_SYSTEM}")
message(STATUS "System name      : ${CMAKE_SYSTEM_NAME}")
message(STATUS "System ver       : ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Conan compiler   : ${CONAN_SETTINGS_COMPILER} ${CONAN_SETTINGS_COMPILER_VERSION}")
message(STATUS "Conan libcxx     : ${CONAN_SETTINGS_COMPILER_LIBCXX}")
message(STATUS)

# The main library
add_library(val_async_cpp INTERFACE)

# Set the version and current build date
set(val_async_cpp_VERSION "${PROJECT_VERSION}")
set(val_async_cpp_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
string(TIMESTAMP val_async_cpp_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP val_async_cpp_BUILD_YEAR "%Y")

message(STATUS "Library ver      : ${val_async_cpp_VERSION}")
message(STATUS "Build date       : ${val_async_cpp_BUILD_DATE}")
message(STATUS "Build year       : ${val_async_cpp_BUILD_YEAR}")
message(STATUS)

# Set library version
set_target_properties(val_async_cpp PROPERTIES
                      VERSION "${val_async_cpp_VERSION}"
                      SOVERSION "${val_async_cpp_VERSION_MAJOR}")

# Declare the public include directories
target_include_directories(val_async_cpp INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
                           $<INSTALL_INTERFACE:include>
                           )

# Ensure that we link with the threading library
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
target_link_libraries(val_async_cpp INTERFACE Threads::Threads)
# target_link_libraries(val_async_cpp INTERFACE CONAN_PKG::boost_context)

# Use C++20 standard
target_compile_features(val_async_cpp INTERFACE cxx_std_20)

# Turn all warnings
target_compile_options(val_async_cpp INTERFACE
                       $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                       -Wall>
                       $<$<CXX_COMPILER_ID:MSVC>:
                       /W4>)
# template backtrace limit
target_compile_options(val_async_cpp INTERFACE
                       $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                       -ftemplate-backtrace-limit=0>
                       )
# Proper use of libc++
if (CONAN_SETTINGS_COMPILER_LIBCXX STREQUAL "libc++")
    target_compile_options(val_async_cpp INTERFACE -stdlib=libc++)
    target_link_options(val_async_cpp INTERFACE -lc++)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
    add_compile_options(-Wno-non-template-friend)
    add_compile_options(-fcoroutines)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif ()

add_library(val_async_cpp::val_async_cpp ALIAS val_async_cpp)


# # Now, set up test executable
# enable_testing()

# set(test_sourceFiles
#     test/test_main.cpp
#     )

# add_executable(test.val_async_cpp ${test_sourceFiles})

# target_include_directories(test.val_async_cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(test.val_async_cpp val_async_cpp)
# target_link_libraries(test.val_async_cpp CONAN_PKG::catch2)

# # Discover the Catch2 test built by the application
# include(CTest)
# include(Catch)
# catch_discover_tests(test.val_async_cpp)


# Set up examples
function(def_example target sourceFile)
    add_executable(${target} ${sourceFile})
    target_link_libraries(${target} val_async_cpp)
    target_link_libraries(${target} CONAN_PKG::boost)
    endfunction()

def_example(example.hello_world "examples/hello_world.cpp")
def_example(example.boost.usage "examples/boost/usage.cpp")
def_example(example.boost.param_passing "examples/boost/param_passing.cpp")
def_example(example.boost.exceptions "examples/boost/exceptions.cpp")
def_example(example.boost.exceptions2 "examples/boost/exceptions2.cpp")
# def_example(example.boost.stack_unwinding "examples/boost/stack_unwinding.cpp")
def_example(example.boost.invert_control "examples/boost/invert_control.cpp")
