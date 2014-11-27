#ifndef PRODCON_H
#define PRODCON_H

#include <stdlib.h>
#include <iostream>       // std::cin, std::cout
#include <queue>          // std::queue
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

// Queue class that has thread synchronisation
template <typename T>
class SynchronisedQueue
{
  private:
    std::queue<T> m_queue; // Use STL queue to store data
    boost::mutex m_mutex; // The mutex to synchronise on
    boost::condition_variable m_cond; // The condition to wait for
   
  public:
    // Add data to the queue and notify others
    void Enqueue(const T& data)
    {
    // Acquire lock on the queue
    boost::unique_lock<boost::mutex> lock(m_mutex);
     
    // Add the data to the queue
    m_queue.push(data);
     
    // Notify others that data is ready
    m_cond.notify_one();
     
    } // Lock is automatically released here
     
    // Get data from the queue. Wait for data if not available
    T Dequeue()
    {
     
    // Acquire lock on the queue
    boost::unique_lock<boost::mutex> lock(m_mutex);
     
    // When there is no data, wait till someone fills it.
    // Lock is automatically released in the wait and obtained
    // again after the wait
    while (m_queue.size()==0) m_cond.wait(lock);
     
    // Retrieve the data from the queue
    T result=m_queue.front(); m_queue.pop();
    return result;
     
    } // Lock is automatically released here
};

// Class that produces objects and puts them in a queue
class Producer
{
  private:
    int m_id; // The id of the producer
    SynchronisedQueue<string>* m_queue; // The queue to use
     
  public:   
    // Constructor with id and the queue to use
    Producer(int id, SynchronisedQueue<string>* queue)
    {
      m_id=id;
      m_queue=queue;
    }
     
    // The thread function fills the queue with data
    void operator () ()
    {
      int data=0;
      while (true)
      {
        // Produce a string and store in the queue
        string str = "Producer: " + boost::lexical_cast<std::string>(m_id) +
        " data: " + boost::lexical_cast<std::string>(data++);
        m_queue->Enqueue(str);
        cout<<str<<endl;
         
        // Sleep one second
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
    }
};

// Class that consumes objects from a queue
class Consumer
{
  private:
    int m_id; // The id of the consumer
    SynchronisedQueue<string>* m_queue; // The queue to use
     
  public:
    // Constructor with id and the queue to use.
    Consumer(int id, SynchronisedQueue<string>* queue)
    {
      m_id=id;
      m_queue=queue;
    }
     
    // The thread function reads data from the queue
    void operator () ()
    {
      while (true)
      {
        // Get the data from the queue and print it
        cout<<"Consumer "<<boost::lexical_cast<std::string>(m_id).c_str()<<" consumed: ("<<m_queue->Dequeue().c_str();
         
        // Make sure we can be interrupted
        boost::this_thread::interruption_point();
      }
    }
};

#endif // PRODCON_H
