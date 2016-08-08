#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

#include <structs.h>
#include <functions.h>
#include <sync.h>

auto http_request_scheduler() -> void {
 
  std::this_thread::sleep_for (std::chrono::seconds(options.time));

  //Signal worker thread to process a request from the queue
  //if(options.debug_flag) std::cout << "(Scheduler Thread): Starting scheduler\n";  
  while(true){
    std::this_thread::sleep_for (std::chrono::seconds(1));
    if(!http_request_queue.empty()){    
      std::unique_lock<std::mutex> data_lock (data_mutex);  
      //if(options.debug_flag) std::cout << "(Scheduler Thread): Sending job to worker thread\n";
      //Reschedule (sort) all the requests in the queue using custom Comparator
      sort(http_request_queue.begin(), http_request_queue.end(), Comparator(options.sched));
      //Signal worker thread
      work_cv.notify_one();            
    }
  }
  
}
