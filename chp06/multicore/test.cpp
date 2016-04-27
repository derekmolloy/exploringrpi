#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
using namespace std;
#define NUM_LOOPS 5000000

void* thread_function(void*)
{
   unsigned rand_seed = 0;
   for(int i=0; i<NUM_LOOPS; i++)
      rand_r(&rand_seed);
   return 0;
}

void random_generate_no_threads()
{
    thread_function(NULL);
    thread_function(NULL);
    thread_function(NULL);
    thread_function(NULL);
}

void random_generate_with_threads()
{
    pthread_t* t1 = new pthread_t;
    pthread_t* t2 = new pthread_t;
    pthread_t* t3 = new pthread_t;
    pthread_t* t4 = new pthread_t;
    pthread_attr_t* attr = new pthread_attr_t;

    pthread_attr_init(attr);
    pthread_attr_setscope(attr, PTHREAD_SCOPE_SYSTEM);

    pthread_create(t1,attr,thread_function,0);
    pthread_create(t2,attr,thread_function,0);
    pthread_create(t3,attr,thread_function,0);
    pthread_create(t4,attr,thread_function,0);

    pthread_join(*t1,0);
    pthread_join(*t2,0);
    pthread_join(*t3,0);
    pthread_join(*t4,0);
    pthread_attr_destroy(attr);
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete attr;
}

int main(void)
{
   auto start = std::chrono::high_resolution_clock::now();
   random_generate_no_threads();
   auto finish = std::chrono::high_resolution_clock::now();
   std::cout << "No threads  " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << " ns" << endl;

   start = std::chrono::high_resolution_clock::now();
   random_generate_with_threads();
   finish = std::chrono::high_resolution_clock::now();
   std::cout << "With threads " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << " ns" << endl;
   return 0;
}
