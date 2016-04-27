/***
 * perftest_boost.cpp A simple threaded performance test that provides indicative performance
 * results on a single core versus a multi core processor using pthreads using Boost
 * Written by Derek Molloy for the book "Exploring Raspberry Pi"
 * This example uses the C++ chrono library that was introduced in C++11, so please
 * compile using: g++ perftest.cpp -o perftest -pthread -stdc++0x
 */

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
using namespace std;
using namespace boost::chrono;
#define NUM_RANDS 5000000

void* thread_function(void*)
{
   unsigned rand_seed = 0;
   for(int i=0; i<NUM_RANDS; i++)
      rand_r(&rand_seed);
   return 0;
}

void random_generate_no_threads(int numCalls)
{
   for(int i=0; i<numCalls; i++){
      thread_function(NULL);
   }
}

void random_generate_with_threads(int numCalls)
{
   pthread_t* threads[numCalls];
   for(int i=0; i<numCalls; i++){
      threads[i] = new pthread_t;
   }
   pthread_attr_t* attr = new pthread_attr_t;
   pthread_attr_init(attr);
   pthread_attr_setscope(attr, PTHREAD_SCOPE_SYSTEM);
   for(int i=0; i<numCalls; i++){
      pthread_create(threads[i],attr,thread_function,0);
   }
   for(int i=0; i<numCalls; i++){
      pthread_join(*threads[i],0);
   }
   pthread_attr_destroy(attr);
   for(int i=0; i<numCalls; i++){
      delete threads[i];            // cannot used delete[]
   }
   delete attr;
}

int main(int argc, char* argv[])
{
   int numCalls = 1;
   if (argc==2){
      int passed = atoi(argv[1]);
      if (passed==0){
         cout << "Invalid number passed to the program -- exiting!" << endl;
         return -EINVAL;    //invalid argument
      }
      else { numCalls = passed; }
   }

   cout << "Boost: There are " << boost::thread::hardware_concurrency() << " threads available" << endl;
   cout << "Performing test using " << numCalls << " thread enabled function calls" << endl;

   process_user_cpu_clock::time_point ubegin = process_user_cpu_clock::now();
   high_resolution_clock::time_point begin = high_resolution_clock::now();
   random_generate_no_threads(numCalls);
   high_resolution_clock::time_point end = high_resolution_clock::now();
   process_user_cpu_clock::time_point uend = process_user_cpu_clock::now();
   std::cout << "Real Time: No threads   " << duration_cast<microseconds>(end-begin) << endl;
   std::cout << "User Time: No threads   " << duration_cast<microseconds>(uend-ubegin) << endl;

   ubegin = process_user_cpu_clock::now();
   begin = high_resolution_clock::now();
   random_generate_with_threads(numCalls);
   end = high_resolution_clock::now();
   uend = process_user_cpu_clock::now();
   std::cout << "Real Time: With threads " << duration_cast<microseconds>(end-begin) << endl;
   std::cout << "User Time: With threads " << duration_cast<microseconds>(uend-ubegin) << endl;

   return 0;
}
