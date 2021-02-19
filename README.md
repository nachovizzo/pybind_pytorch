# Minimum Viable PyTorch C++ - Python Binding

This is a very minimalistic project that aims to build a C++/Python module,
leveraging on the power of PyTorch tools.

## Motivation

This project is actually **NOT** about PyTorch or any Deep Learning Framework
in general. I have used `pybind11` in the past and I think is an awesome tool
and I really like it. But not so much time ago, I stumbled across someone that
wrote a PyTorch CUDA extension. And the few lines of code that it's needed to
build the C++ and bind it to a python module was impressing to me(in our case
is just `5` lines of code)

Of course that not many people know what's going on under the hood. But
basically PyTorch people provided an awesome wrapper
(`torch.utils.cpp_extension`) that does all the usual black magic you need to
do when binding C++ modules using `CMake` and `setuptools`.

If you don't believe me then you could look into
[pybind11/cmake_example](https://github.com/pybind/cmake_example) and see that
there, it's much more than `5` lines of code.

Of course if you open the lid and go into the `BuildExtension` and
`CppExtension` classes, they are just wrappers for `setuptools.Extension` and
`setuptools.command.build_ext`. But in any case it makes the binding so much
easy.
