Python C extensions
===================

An interesting feature offered to developers by the CPython implementation is the ease of interfacing C code to Python.

There are three key methods developers use to call C functions from their python code - `ctypes`, `SWIG` and `Python/C API`. Each method comes with it's own merits and demerits.

Firstly, why would you want to interface C with Python?
A few common reasons are :
* You want speed and you know C is about 50x faster than Python.
* Certain legacy C libraries work just as well as you want them to, so you don't want to rewrite them in python.
* Certain low level resource access - from memory to file interfaces.
* Just because you want to.
