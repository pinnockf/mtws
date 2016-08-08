
// CSE421 Summer 2016           Team name: MikeJoelFrank
// University at Buffalo        June 24, 2016 
// 
//                    Team Members:
//
// Name:               Email:                   UB#
// Franklin Pinnock    fgpinnoc@buffalo.edu     36398302
// Joel Little         joellitt@buffalo.edu     36183498          
// Michael Mastro      mmastro@buffalo.edu      50035644

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>

#include <ctime>

#include <structs.h>
#include <functions.h>
#include <options.h>
#include <server_init.h>
#include <queue.h>
#include <scheduler.h>
#include <action.h>
#include <sync.h>


std::mutex data_mutex, work_mutex, log_mutex;
std::condition_variable_any work_cv;

server_options options;
std::vector<http_request_data> http_request_queue;


int main(int argc, char* argv[]){   

    //Use info flag to show useful output about program
    options.info_flag = false;
    
    //Set default options 
    set_default_server_options(options);


    char option;
    while((option = getopt(argc,argv,"hdl:p:r:t:n:s:"))!=-1){
      switch(option){
        case 'h':
          show_help();
          return 0;
          break;
        case 'd':
          options.debug_flag = true;
          break;
        case 'l':
          options.file = optarg;
          options.logging_enabled = true;
          break;
        case 'p':   
          options.port = std::stoi(optarg, nullptr, 10);
          break;
        case 'r':
          options.dir = optarg;
          chdir(optarg);
        break;     
        case 't':  
          options.time = std::stoi(optarg, nullptr, 10); 
          break;     
        case 'n':
          options.threadnum = std::stoi(optarg, nullptr, 10);   
          break;        
        case 's': 
          options.sched = optarg;  
          break;
        default:
        break;
      }
    }
  
    if (options.debug_flag == false){
        daemon(1, 0);  
    }
  
    
    //At this point options struct contains all options to be used in program.          
    if(options.info_flag == true) show_server_info(options);
    
    //Initialize server
    int server_socket_fd = initialize_server(options);
        
    //Main thread creates queueing thread
    std::thread queue_thread(http_request_handler, std::cref(server_socket_fd));
        
    //Create the scheduling thread (the producer)
    //if(options.info_flag == true) std::cout <<"(Main thread): Creating scheduling thread\n";
    std::thread scheduler_thread(http_request_scheduler);
    
                             
    //Create worker threads (the consumers) from -n option
    //if(options.info_flag == true) std::cout << "(Main thread): Creating worker threads\n";
    std::vector<std::thread> threads;
    threads.reserve(options.threadnum); 
    
    for(int thread_id = 1 ; thread_id <= options.threadnum; thread_id++ ) {
      threads.emplace_back(process_queue_block, thread_id);
      std::this_thread::sleep_for (std::chrono::seconds(1));
    }
    

    queue_thread.join();
    scheduler_thread.join();
    for(auto &thread : threads){
      thread.detach();
    }

    return 0;
}
