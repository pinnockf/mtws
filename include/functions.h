#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <structs.h>

void log_request();     
std::string get_time();                                     // Returns the current GMT time as a string   
bool is_directory(std::string);   
std::string file_last_modified(std::string);              
int file_size(std::string);     

std::string file_content_type(std::string);   
void display_html(FILE*, std::string);                      // Display HTML using out stream pointer & resource string   
void data_to_client(FILE*, std::string, std::string, int&, int&); // File pointer, request commend, request path, #content_bytes   
                         

auto display_block_contents(http_request_data&) -> void;
auto display_queue_contents(std::vector<http_request_data>&) -> void; 
auto parse_http_request(const std::string&) -> std::vector<std::string>; 
auto get_exe_path() -> std::string;    
auto log_http_request(const http_request_data&, const server_options&) -> void;                  
auto print_current_directory(const std::string&) -> void;

#endif
