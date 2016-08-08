#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

//Stores server options
struct server_options {
    bool info_flag;    
    bool debug_flag;
    bool logging_enabled;
    std::string file;   // input file for logging output
    int port;
    std::string dir;
    int time;
    int threadnum;
    std::string sched;
};

//Stores http request and assoicated data
struct http_request_data {
  int client_write_fd = 0;
  std::string http_request;
  std::string request_type;
  std::string ip_address;
  int status_code = 0;
  int content_length = 0;
  int queue_position = 0;
  std::string time_received;
  std::string time_sent;
};

//Comparator that will sort the queue and put the lowest job in the back
struct Comparator {
  std::string sched;
  Comparator(std::string _sched) : sched(_sched){}
  bool operator()(http_request_data& queue_block1, http_request_data& queue_block2){
    if(sched == "FCFS"){
      if(queue_block1.queue_position > queue_block2.queue_position) return true;
    }
    if(sched == "SJF"){
      if(queue_block1.content_length > queue_block2.content_length) return true;
      if(queue_block1.content_length == queue_block2.content_length
        &&queue_block1.queue_position > queue_block2.queue_position) return true;
      return false;
    }
  } 
}; 

#endif
