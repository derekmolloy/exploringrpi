/* Example C program that can be called from within Python. Written
 * by Derek Molloy for the book "Exploring Raspberry Pi"
 */
#include <Python.h>
#include <math.h>

/** A hello() function that can be called from Python:
 *  @param self A pointer to the calling PyObject
 *  @param args the arguments passed from the Python code
 *  @return All objects types extend PyObject -- return a ptr
*/
static PyObject* hello(PyObject* self, PyObject* args){
   //If a name is passed use it, otherwise say "Hello World!"
   const char* name;
   if (!PyArg_ParseTuple(args, "s", &name)){
      printf("Hello World!\n");
      Py_RETURN_NONE;
   }
   printf("Hello %s!\n", name);
   Py_RETURN_NONE;
}

/** integrate() function to integrate sin(x) over a range a..b*/
static PyObject* integrate(PyObject* self, PyObject* args){
   double a, b, dx, sum=0;
   int N;
   // expecting two doubles and an int from Python
   if (!PyArg_ParseTuple(args, "ddi", &a, &b, &N)){
      printf("Failed to parse the arguments!\n");
      Py_RETURN_NONE;
   }
   dx = (b-a)/N;
   for(int i=0; i<N; i++){
      sum += sin((a+i)*dx);
   }
   return Py_BuildValue("d", sum*dx); // send PyObject back to Python
}

/** An array of structures, where each structure has four fields:
 *   ml_name  (char *) The name of the function
 *   ml_meth  (PyCFunction) A pointer to the C function above
 *   ml_flags (int) flag bits that state how the call is constructed
 *   ml_doc   (char *) Describes the function
 * hello and integrate are exposed in this example.*/
static PyMethodDef ERPiMethods[] = {
   {"hello", hello, METH_VARARGS, "Displays Hello Derek!"},
   {"integrate", integrate, METH_VARARGS, "Integrates the sin(x) fn."},
   {NULL, NULL, 0, NULL}              // must end with a null structure
};

/** Initialization function that defines available modules
 */
PyMODINIT_FUNC initERPiModule(void){
   (void) Py_InitModule("ERPiModule", ERPiMethods);
}
