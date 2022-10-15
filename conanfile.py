from conans import ConanFile, CMake, tools
import re
from os import path


class CppAsyncRecipe(ConanFile):
    name = "ValCppAsync"
    description = "POC with C++ for Val concurrency"
    author = "Lucian Radu Teodorescu"
    topics = ("Val", "concurrency")
    homepage = "https://github.com/lucteo/val_cpp_async"
    url = "https://github.com/lucteo/val_cpp_async"
    license = "MIT License"
    settings = "compiler", "cppstd"  # Header only - compiler only used for flags
    tool_requires = "catch2/2.13.6", "boost/1.79.0"
    exports_sources = "include/*"
    generators = "cmake"

    # def validate(self):
    #     tools.check_min_cppstd(self,"20")

    def set_version(self):
        self.version = f"0.1.0"

    def package(self):
        self.copy("*.hpp")

    def package_info(self):
        # Make sure to add the correct flags for gcc
        if self.settings.compiler == "gcc":
            self.cpp_info.cxxflags = ["-fcoroutines", "-Wno-non-template-friend"]


# from <root>/.build/ directory, run:
#   > conan install .. --build=missing -s cppstd=20