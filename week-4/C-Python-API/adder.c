//The C file that uses the Python C api to add all numbers in a list

//Python.h has all the required function definitions to manipulate the Python objects
#include <Python.h>

 //This is the function that is called from your python code
static PyObject* addList_add(PyObject* self, PyObject* args){

  PyObject * listObj;

  //The input arguments come as a tuple, we parse the args to get the various variables
  //In this case it's only one list variable, which will now be referenced by listObj
  if (! PyArg_ParseTuple( args, "O!", &PyList_Type, &listObj))
    return NULL;

  //length of the list
  long length = PyList_Size(listObj);

  //iterate over all the elements
  int i, sum =0;
  for(i = 0; i < length; i++){
    //get an element out of the list - the element is also a python objects
    PyObject* temp = PyList_GetItem(listObj, i);
    //we know that object represents an integer - so convert it into C long
    long elem = PyInt_AsLong(temp);
    sum += elem;
  }

  //value returned back to python code - another python object
  //build value here converts the C long to a python integer
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
