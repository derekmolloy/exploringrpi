#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

// This is the thread function that will execute when the thread is created
//  it passes and receives data by void pointers
void *threadFunction(void *value){
   int *x = (int *)value;    //cast the data passed to an int pointer
   while(*x<5){              //while the value of x is less than 5
      usleep(10);            //sleep for 10us - encourage main thread
      (*x)++;                //increment the value of x by 1
   }
   return x;                 //return the pointer x (as a void*)
}

int main() {
   int x=0, y=0;
   pthread_t thread;        //this is our handle to the pthread
   // create the thread, pass the reference, address of the function and data
   // pthread_create() returns 0 on the successful creation of a thread
   if(pthread_create(&thread, NULL, &threadFunction, &x)!=0){
      cout << "Failed to create the thread" << endl;
      return 1;
   }
   // at this point the thread was created successfully
   while(y<5){              // loop and increment y, displaying values
      cout << "The value of x=" << x << " and y=" << y++ << endl;
      usleep(10);           // encourage the pthread to run
   }
   void* result;            // OPTIONAL: receive data back from pthread
   pthread_join(thread, &result);   // allow the pthread to complete
   int *z = (int *) result;         // cast from void* to int* to get z
   cout << "Final: x=" << x << ", y=" << y << " and z=" << *z << endl;
   return 0;
}
