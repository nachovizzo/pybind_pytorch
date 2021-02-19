// This should be in $HOME/.local/lib/python3.8/site-packages/torch/include/
#include <pybind11/pybind11.h>

#include <iostream>

void Greet() { std::cout << "Hello Pytorch from C++" << std::endl; }
PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) { m.def("greet", &Greet); }
