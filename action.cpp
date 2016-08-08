#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>

#include <structs.h>
#include <functions.h>
#include <sync.h>

//Need log mutex

auto process_queue_block(const int thread_id) -> void{
    
    //if(options.info_flag == true) std::cout << "(Worker Thread " << thread_id << "): Ready and waiting for signal\n";  
    
    while(true){
      http_request_data queue_block;            
      //Wait for signal from scheduler thread
      std::unique_lock<std::mutex> work_lock (work_mutex);
      work_cv.wait(work_lock);      
        {
          //Lock the queue while removing a queue_block
          std::unique_lock<std::mutex> data_lock (data_mutex);
          //if(options.info_flag == true) std::cout << "(Worker Thread " << thread_id << "): Signal Received. Processing Request\n";
          
          //Extract block and remove it from the queue
          //display_queue_contents(http_request_queue);
          queue_block = http_request_queue.back();         
          http_request_queue.pop_back();
        } 
          
      //At this point the queue_block struct has all the info    
  
      //Write to client
      
      int client_write_fd = queue_block.client_write_fd;
      FILE* client_write_fp = fdopen(client_write_fd,"w");
      if(client_write_fp == NULL){
        perror("Can't open file descriptor");
        exit(1);
      }
      
      //At this point you can:.
      //Write to client with client_write_fp.
      setbuf(client_write_fp, NULL);  
      //if(options.info_flag == true) fprintf(client_write_fp, "> MJF Server (Worker Thread %i): Finished Processing.\n", thread_id);
      std::vector<std::string> http_request_info;
      http_request_info = parse_http_request(queue_block.http_request);
      std::string http_request_type = http_request_info[0];
      std::string http_request_path = http_request_info[1];


      if (http_request_path[0] == '/'){
        http_request_path = http_request_path.substr(1);
      }
       // Send data to client to be rendered     
      data_to_client(client_write_fp, http_request_type, http_request_path, queue_block.content_length, queue_block.status_code);    

      //Log time sent and then append info to log file
      queue_block.time_sent = get_time();
      fclose(client_write_fp);

      // Acquire lock while writing to log file
      {
        std::unique_lock<std::mutex> log_lock (log_mutex);
          log_http_request(queue_block, options);       
      }
      
      std::this_thread::sleep_for (std::chrono::seconds(1));
    }

}
