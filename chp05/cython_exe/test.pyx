cdef extern from "math.h":
   double sin(double x)

cpdef double integrate_sin(double a, double b, int N):
   cdef double dx, s
   cdef int i
   dx = (b-a)/N
   sum = 0
   for i in range(0,N):
      sum += sin(a+i*dx)
   return sum*dx

if __name__ == '__main__':
   integral = integrate_sin(0, 3.14159, 1000000)
   print("The integral of sin(x) in the range 0..PI is: ", integral)
