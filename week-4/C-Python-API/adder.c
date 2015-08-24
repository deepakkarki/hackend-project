//The C file that uses the Python C api to add all numbers in a list

#include <Python.h>

static PyObject* addList_add(PyObject* self, PyObject* args){
  //logic here
  int sum = 0;
  return Py_BuildValue("i", sum);
}

static char addList_docs[] =
    "add( ): add all elements of the list\n";

static PyMethodDef addList_funcs[] = {
    {"add", (PyCFunction)addList_add, METH_VARARGS, addList_docs},
    {NULL, NULL, 0, NULL}
};

/*
addList is the module name
*/
PyMODINIT_FUNC initaddList(void){
    Py_InitModule3("addList", addList_funcs,
                   "Add all ze lists");
}
