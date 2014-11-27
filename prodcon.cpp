#include <stdlib.h>
#include <boost/thread.hpp>
#include "prodcon.hpp"

using namespace std;
int main()
{
  // Display the number of processors/cores
  cout<<boost::thread::hardware_concurrency()
  <<" processors/cores detected."<<endl<<endl;
  cout<<"When threads are running, press enter to stop"<<endl;
   
  // The number of producers/consumers
  int nrProducers, nrConsumers;
   
  // The shared queue
  SynchronisedQueue<string> queue;
   
  // Ask the number of producers
  cout<<"How many producers do you want? : ";
  cin>>nrProducers;
   
  // Ask the number of consumers
  cout<<"How many consumers do you want? : ";
  cin>>nrConsumers;
   
  // Create producers
  boost::thread_group producers;
  for (int i=0; i<nrProducers; i++)
  {
    Producer p(i, &queue);
    producers.create_thread(p);
  }
   
  // Create consumers
  boost::thread_group consumers;
  for (int i=0; i<nrConsumers; i++)
  {
    Consumer c(i, &queue);
    consumers.create_thread(c);
  }
  // Wait for enter (two times because the return from the
  // previous cin is still in the buffer)
  getchar(); getchar();
   
  // Interrupt the threads and stop them
  producers.interrupt_all(); producers.join_all();
  consumers.interrupt_all(); consumers.join_all();
} 