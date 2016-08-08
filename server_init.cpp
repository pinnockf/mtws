#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <structs.h>
#include <functions.h>


int initialize_server(const server_options& options){
  //Create socket
  //if(options.info_flag == true) std::cout << "(Main thread): Creating Socket" << std::endl;
  int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
  if(server_socket_fd < 0){
    perror("Socket creation failed");
    exit(1);	
  }
  
  //Populate struct
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(options.port);
  
  //Bind socket to port
  //f(options.info_flag == true) std::cout << "(Main thread): Binding socket to port " << options.port << std::endl;   
  int bind_result_code = bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
  if(bind_result_code != 0){
    perror("Bind failed");
    close(server_socket_fd);
    exit(1);
  }
  
  //Listen on port for incoming connections
  int listen_queue = options.threadnum;
  if(options.debug_flag) listen_queue = 1; 
  int listen_result_code = listen(server_socket_fd, 5);
  if(listen_result_code != 0){
    perror("Listen failed");
    close(server_socket_fd);
    exit(1);      
  }

  return server_socket_fd;  
}
