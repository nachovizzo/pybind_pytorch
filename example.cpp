#include <torch/python.h>

#include <iostream>

void Greet() { std::cout << "Hello Pytorch from C++" << std::endl; }
PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) { m.def("greet", &Greet); }
