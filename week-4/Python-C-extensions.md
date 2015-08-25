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

##1. CTypes

The Python [ctypes module](https://docs.python.org/2/library/ctypes.html) is probably the most easiest way to call C functions from Python. The ctypes module provides C compatible data types and functions to load DLLs so that calls can be made to C shared libraries without having to modify them. The fact that the C side needn't be touched adds to the simplicity of this method.

Example :

Simple C code to add two numbers, save it as `add.c`

```c
//sample C file to add 2 numbers - int and floats

#include <stdio.h>

int add_int(int, int);
float add_float(float, float);

int add_int(int num1, int num2){
	return num1 + num2;
}

float add_float(float num1, float num2){
	return num1 + num2;
}
```

Next compile the C file to a `.so` file (DLL in windows)
This will generate a adder.so file.
```bash
#For Linux
$  gcc -shared -Wl,-soname,adder -o adder.so -fPIC add.c

#For Mac
$ gcc -shared -Wl,-install_name,adder.so -o adder.so -fPIC add.c
```
Now in your python code -

```python
from ctypes import *

#load the shared object file
adder = CDLL('./adder.so')

#Find sum of integers
res_int = adder.add_int(4,5)
print "Sum of 4 and 5 = " + str(res_int)

#Find sum of floats
a = c_float(5.5)
b = c_float(4.1)

add_float = adder.add_float
add_float.restype = c_float
print "Sum of 5.5 and 4.1 = ", str(add_float(a, b))
```

And the output is as follows

```
Sum of 4 and 5 = 9
Sum of 5.5 and 4.1 =  9.60000038147
```

In this example the C file is self explanatry - it contains two functions, one to add `int`s and another to add `float`s.

In the python file, first the ctypes module is imported. Then the CDLL function of the ctypes module is used to load the shared lib file we created. The functions defined in the C lib is now available to us via the `adder` variable. When `adder.add_int()` is called, internally a call is made to the `add_int` C function. The ctypes interface allows us to use native python integers and strings while calling the C functions.

For other types such as boolean or float, we have to use the correct ctypes. This is seen while passing parameters to the `adder.add_float()`. We first create the required c_float types from python deciaml values, and then use them as arguments to the C code.
This method is simple and clean, but limited. For example it's not possible to manipulate objects on the C side.

##2. SWIG

Simplified Wrapper and Interface Generator, or SWIG for short is aother way to interface C code to Python. In this method, the developer must develop an extra interface file which is an input to SWIG (the command line utility).

Python developers generally don't use this method, because it is in most cases unnecessarily complex. This is a great method when you have a C/C++ code base, and you want to interface it to many different languages.

Example from the SWIG website :

The C code, `example.c` that has a variety of functions and variables

```c
 #include <time.h>
 double My_variable = 3.0;

 int fact(int n) {
     if (n <= 1) return 1;
     else return n*fact(n-1);
 }

 int my_mod(int x, int y) {
     return (x%y);
 }

 char *get_time()
 {
     time_t ltime;
     time(&ltime);
     return ctime(&ltime);
 }
```

The interface file - this will remain the same irrespective of the language you want to port your C code to :

```
/* example.i */
 %module example
 %{
 /* Put header files here or function declarations like below */
 extern double My_variable;
 extern int fact(int n);
 extern int my_mod(int x, int y);
 extern char *get_time();
 %}

 extern double My_variable;
 extern int fact(int n);
 extern int my_mod(int x, int y);
 extern char *get_time();
```
And now to compile it

```
unix % swig -python example.i
unix % gcc -c example.c example_wrap.c \
        -I/usr/local/include/python2.1
unix % ld -shared example.o example_wrap.o -o _example.so
```

Finally, the Python output

```python
>>> import example
>>> example.fact(5)
120
>>> example.my_mod(7,3)
1
>>> example.get_time()
'Sun Feb 11 23:01:07 1996'
>>>
```

##3. Python/C API

This is probably the most widely used method - not for it's simplicity but for the fact that you can manipulate python objects in your C code.

This method requires your C code to be specifically written for interfacing with Python code. All Python objects are represented as a PyObject struct and the `Python.h` header file provides various functions to manipulate it.
For example if the PyObject is also a PyListType (basically a list), then we can use the `PyList_Size()` function on the struct to get the length of the list. This is equivalent to calling `len(list)` in python. Most of the basic functions/opertions that are there for native Python objects are made avialble in C via the `Python.h` header.
