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
