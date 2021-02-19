from setuptools import setup
from torch.utils.cpp_extension import BuildExtension, CppExtension

setup(name='test_pybind_torch',
      ext_modules=[CppExtension('pybind_torch', ['pybind_torch.cpp'])],
      cmdclass={'build_ext': BuildExtension})
