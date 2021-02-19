# Minimum Viable PyTorch C++ - Python Binding

- [Minimum Viable PyTorch C++ - Python Binding](#minimum-viable-pytorch-c---python-binding)
  - [How to build and run this](#how-to-build-and-run-this)
  - [Motivation](#motivation)
  - [Drawbacks](#drawbacks)
  - [Benefits](#benefits)
    - [Simplicity](#simplicity)
    - [Packaging](#packaging)
  - [Troubleshooting](#troubleshooting)

This is a very minimalistic project that aims to build a C++/Python module,
leveraging on the power of PyTorch tools.

## How to build and run this

This is the **most** exciting part. Just clone this repo and run this command

```sh
pip3 install -e . -r requirements.txt
```

This will basically build the `C++` extension that in my case looks something
like `pybind_torch.cpython-38-x86_64-linux-gnu.so` and install it on `develop`
mode. This way you only copy an `.egg-link` file in your local `site-packages`
repositories. If you are curious abut this process, just Google it, the same I
did. But basically this won't install anything in your system, but yet you are
able to open a python shell and run something like this:

```python
from pybind_torch import greet; greet()
```

If everything is ok you should get this on your shell:

```shell
Hello Pytorch from C++
```

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

## Drawbacks

Of course the main drawbacks of this approach are two

- You need to install `torch`, although you can do just `pip3 install torch`
- If you don't have properly installed the `libtorch` library, this won't work.
  Even when you don't use this library at all!
- You will link the binded Python module to all the C++ torch libraries, which
  you might not want.

The last one is the most "important" to me. Since, if you are trying to do any
serious development, this is something you must avoid. If you inspect the
libraries linked to the python module you can find all common torch
dependencies, which we don't even use in this case:

```shell
ldd pybind_torch.cpython-38-x86_64-linux-gnu.so
    linux-vdso.so.1 (0x00007ffcc4fd6000)
    libc10.so => /home/ivizzo/dev/libs/libtorch/lib/libc10.so (0x00007f0ce05f3000) # <---NOT USED
    libtorch_cpu.so => /home/ivizzo/dev/libs/libtorch/lib/libtorch_cpu.so (0x00007f0ccf8bd000)# <---NOT USED
    libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f0ccf690000)
    libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f0ccf675000)
    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f0ccf483000)
    libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f0ccf334000)
    libgomp-75eea7e8.so.1 => /home/ivizzo/dev/libs/libtorch/lib/libgomp-75eea7e8.so.1 (0x00007f0ccf10d000)# <---NOT USED
    libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f0ccf0ea000)# <---NOT USED
    /lib64/ld-linux-x86-64.so.2 (0x00007f0ce088c000)
    librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f0ccf0df000)# <---NOT USED
    libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f0ccf0d9000)# <---NOT USED
    libcudart-3f3c6934.so.11.0 => /home/ivizzo/dev/libs/libtorch/lib/libcudart-3f3c6934.so.11.0 (0x00007f0ccee57000)# <---NOT USED
```

Of course if you inspect the `cpp_extension` module you will find this kind of
lines:

```python
libraries = kwargs.get('libraries', [])
libraries.append('c10')
libraries.append('torch')
libraries.append('torch_cpu')
libraries.append('torch_python')
```

But you could still inspire yourself with the `cpp_extension` module and build
your own.

## Benefits

### Simplicity

Extremely simple, just a few lines of code of calling `C++` modules from Python.
`cloc` reports something like only `11` lines of code for the whole project

```sh
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
Python                           2              1              0              7
C++                              1              2              1              4
-------------------------------------------------------------------------------
SUM:                             3              3              1             11
-------------------------------------------------------------------------------
```

### Packaging

In contrast to the usual `C++` development process this got crazy easy. After
you install the `torch` module with pip, this will also install the headers
files and the necessary python wrappers to do all this magic. You don't even
need to write your own `CMakeLists.txt` file, which I found easy to follow.

When you are extremely curious like me, you could take a look to the
site-packages dir of the `torch` library. In my case I strongly hate `conda` or
any of its variants so I use just plain `pip3` and always install in my local
home directory. So I have these files in:

```shel
$ ls -1 $HOME/.local/lib/python3*/site-packages/torch/include
ATen
c10
c10d
caffe2
pybind11 # <--- look at this guy
TH
THC
THCUNN
torch
```

Pay attention that even `pybind11` has been included in the package, so you
don't even need to clone that library.

## Troubleshooting

I guess the only real problem you might see is this one:

```shell
 python3 example.py
Traceback (most recent call last):
  File "example.py", line 1, in <module>
    from pybind_torch import greet
ImportError: libc10.so: cannot open shared object file: No such file or directory
```

This basically means that you don't have installed the `libtorch` library.
Please read [Drawbacks](#drawbacks).

Google how to do this, in my case was enough to just change my
`LD_LIBRARY_PATH` to where I had this library installed:

```sh
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$HOME/dev/libs/libtorch/lib/
```

There are other workarounds, but that's not the point of this example.
