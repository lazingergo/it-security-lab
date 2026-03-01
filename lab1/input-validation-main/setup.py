from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        'ciff', 
        ['ciff.cpp'], 
        include_dirs=[pybind11.get_include()],
        language='c++',
        extra_compile_args=['-O3', '-std=c++11'], 
    ),
]

setup(
    name='ciff',
    ext_modules=ext_modules,
)
