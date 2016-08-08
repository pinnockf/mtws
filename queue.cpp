#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string.h>

#include <structs.h>
#include <functions.h>
#include <sync.h>
#include <arpa/inet.h>

void http_request_handler(const int& server_socket_fd){

  //if(options.info_flag == true) std::cout << "(Queue thread): Listening on port " << options.port << " for incoming connections\n";

  struct sockaddr_in client_address;
  int client_address_length = sizeof(client_address);
  
  int queue_position = 1;
  while(true){
    int client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &client_address, (socklen_t*) &client_address_length);
    if(client_socket_fd == -1){
      perror("Client socket creatiion failed");
      exit(1);
    }
    
    // Retrieve and store the IP address   
    const char* ip_addr_ptr = inet_ntoa(client_address.sin_addr); 
    std::string ip_address_string(ip_addr_ptr);


    // if(options.info_flag == true) std::cout << "(Queue Thread): Client Connected\n";

    int client_read_fd = client_socket_fd;
    int client_write_fd = dup(client_socket_fd);
          
    //Wrap client File descriptor with file pointers.
    FILE* client_read_fp = fdopen(client_read_fd,"r"); 
    FILE* client_write_fp = fdopen(client_write_fd,"w");
    if(client_read_fp == NULL || client_write_fp == NULL){
      perror("Can't open file descriptor");
      exit(1);
    }
    
    //At this point you can:
    //Read from client with client_read_fp.     
    //Write to client with client_write_fp. 
    if(options.info_flag == true){
      // Uncomment these to see thread dispatching
      // fprintf(client_write_fp, "> MJF Server (Queue Thread): Thanks for connecting to our server.\n");
      // fprintf(client_write_fp, "> MJF Server (Queue Thread): I wil store your request into the request queue.\n"); 
    }
       
  
    setbuf(client_write_fp, NULL); 
    char http_request_buffer[1000];  
    std::string http_request = "";
    std::vector<std::string> http_request_info;
    std::string http_request_type, requested_path, http_version;
 
    //Get request from client, parse it, and store it in the queue.  
    fgets(http_request_buffer, sizeof(http_request_buffer), client_read_fp);
    fclose(client_read_fp);     
      
    http_request = http_request_buffer;     
    std::size_t found = http_request.find("shutdown");
    if (found!=std::string::npos) break;
    
    
    http_request_info = parse_http_request(http_request);  
    http_request_type = http_request_info[0];            // stores the GET or HEAD request type
    requested_path = http_request_info[1];    
    http_version = http_request_info[2];
    
    //Populate struct with http request info block
    http_request_data queue_block;
    queue_block.ip_address = ip_address_string;                                                                    
    queue_block.client_write_fd = client_write_fd;
    queue_block.http_request = http_request;
    
    //std::vectorparse_http_request(http_request);
    
    srand(time(NULL));
    queue_block.status_code = rand() % 50; 

    // Strip forward slash prefix from URL          
    if (requested_path[0] == '/'){     
        requested_path = requested_path.substr(1);
    }
      
    FILE *resource_file_ptr = fopen(requested_path.c_str(), "rb");

    if (resource_file_ptr != NULL){
        queue_block.content_length = file_size(requested_path);
    }
    else {
        queue_block.content_length = 0; 
    }


    queue_block.queue_position = queue_position; queue_position++;
    queue_block.time_received = get_time();
    
     
    //Acquire lock and put request into queue
    {
      std::unique_lock<std::mutex> data_lock (data_mutex);
      http_request_queue.push_back(queue_block); 
    }     
  }
  
}
