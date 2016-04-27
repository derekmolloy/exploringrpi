#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
using namespace boost::geometry::model::d2;
#include <iostream>

int main(){
   point_xy<float> p1(1.0,2.0), p2(3.0,4.0);
   float d = boost::geometry::distance(p1,p2);
   std::cout << "The distance between points is: " << d << std::endl;
   return 0;
}
