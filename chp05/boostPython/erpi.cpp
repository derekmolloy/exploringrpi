#include<string>
#include<boost/python.hpp>             // hpp - used for c++ headers
using namespace std;

namespace exploringrpi{                // keep global namespace clean

   string hello(string name) {         // e.g., returns "Hello Derek!"
      return ("Hello " + name + "!");
   }

   double integrate(double a, double b, int n) {
      double sum=0, dx = (b-a)/n;
      for(int i=0; i<n; i++){ sum += sin((a+i)*dx); }
      return sum*dx;
   }
}

BOOST_PYTHON_MODULE(erpi){            // the module is called erpi
   using namespace boost::python;     // require this namespace
   using namespace exploringrpi;      // bring in custom namespace
   def("hello", hello);               // the hello() function
   def("integrate", integrate);       // the integrate() function
}

