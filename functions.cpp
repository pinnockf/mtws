#include <iostream>
#include <sstream>
#include <vector>
#include <time.h> 
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>

#include <structs.h>
#include <sys/stat.h>
#include <cstring>
#include <string>
#include <fstream>
#include <stdio.h>
#include <unistd.h>

 // TODO: Doesn't work in all cases i.e. if a directory has a period in it
 // Function Returns true if request returns a directory, returns false if it returns a file
 bool is_directory(std::string str){

     std::size_t last_slash_position = str.find_last_of('/');  
     std::size_t last_period_position = str.find_last_of('.');  

     if (last_slash_position == std::string::npos)    // if there's no '/'' set it's size_t to 0
     {
         last_slash_position = 0;    
     }

     if (last_period_position != std::string::npos)   // If we find a '.'
     {
         std::string extension = str.substr(last_period_position+1);    
   
         if (last_period_position > last_slash_position && extension.length() > 0){                                                             
            return false;
        }
     }               

     return true;
 }

// Returns the GMT formatted time as a string
std::string get_time(){

    std::string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    std::string time_string, day, month, year, hours, minutes, seconds;
    time_t epoch_time; 
    struct tm *GMT_time;   // Stores date/time elements
    time(&epoch_time);  
    GMT_time = gmtime(&epoch_time);

    day     = std::to_string(GMT_time->tm_mday);  
    month   = months[GMT_time->tm_mon];
    year    = std::to_string(GMT_time->tm_year+1900);    
    hours   = std::to_string(GMT_time->tm_hour);
    minutes = std::to_string(GMT_time->tm_min); 
    seconds = std::to_string(GMT_time->tm_sec);        

    time_string = day + "/" + month + "/" + year + ":" + hours + ":" + minutes + ":" + seconds;  
    //std::cout << time_string << std::endl;
             
    return time_string;
}

// Pass in a file name and it'll return it's last modified date as a string
std::string file_last_modified(std::string file_string){

    std::string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    std::string time_string, day, month, year, hours, minutes, seconds;
    char file_name[file_string.size()+1];
    std::strcpy(file_name, file_string.c_str()); // String to Char array

    struct stat file_stats;
    stat(file_name, &file_stats);                              
    struct tm *GMT_file_time = gmtime(&file_stats.st_mtime); 

    day     = std::to_string(GMT_file_time->tm_mday);  
    month   = months[GMT_file_time->tm_mon];
    year    = std::to_string(GMT_file_time->tm_year+1900);    
    hours   = std::to_string(GMT_file_time->tm_hour);
    minutes = std::to_string(GMT_file_time->tm_min); 
    seconds = std::to_string(GMT_file_time->tm_sec);        

    time_string = day + "/" + month + "/" + year + ":" + hours + ":" + minutes + ":" + seconds;  

    return time_string;     
}


// Pass in a file name and return file type
/*std::string file_type(std::string file_string){

    len = strlen(file_string);
    std::string type("");
    bool marker = false;

    for (int i=0; i<len; i++;){

        if (marker = true){
            type = type + file_string[i];
        }else{ }

        if (file_string[i] == "."){
            marker = true;
        }else{ }

    }
    return type;
}*/

int file_size(std::string file_string){
    
    unsigned int bytes = 0;
    char file_name[file_string.size()+1];
    std::strcpy(file_name, file_string.c_str());

    struct stat file_stats;
    stat(file_name, &file_stats);
    bytes = file_stats.st_size;                         
    // std::cout << "Byte size is " << bytes << std::endl;

    return bytes;
}

std::string file_content_type(std::string file_string){
    /* This function will work with .txt, .html, .gif, and .jpg/.jpeg files */ 

    std::size_t last_period_position = file_string.find_last_of('.');  

    if (last_period_position != std::string::npos){
        std::string extension = file_string.substr(last_period_position+1);

        char extension_char_array[extension.size()+1];

        if (extension == "txt" || extension == "html"){
            return "text/html";
        } 
        else if (extension == "gif"){
            return "image/gif";
        }
        else if (extension == "jpg" || extension == "jpeg"){
            return "image/jpeg";
        }
        else {
            return "";
        }   
    }

    return "";
}

// Display the content of a http request block
auto display_block_contents(http_request_data& queue_block) -> void {

    std::cout << " Client file descriptor: "<< queue_block.client_write_fd << std::endl;
    std::cout << " HTTP request: "<< queue_block.http_request << std::endl;
    std::cout << " Content length: "<< queue_block.content_length << std::endl;
    std::cout << " Queue position: "<< queue_block.queue_position << std::endl;
    std::cout << " Time Received: "<< queue_block.time_received << std::endl;    
      
}

// Display the content of the http request queue
auto display_queue_contents(std::vector<http_request_data>& http_request_queue) -> void {         
    
    if(!http_request_queue.empty()){
      std::cout << "------------------------\n";
      std::cout << "Request Queue Contents: \n";
      for (auto &queue_block : http_request_queue){
        display_block_contents(queue_block);    
      }
    }   
}

//Parse http request into a vector.
//Position 0 is http_request
//Position 1 is request_path
//Position 2 is http_version
auto parse_http_request(const std::string& http_request) -> std::vector<std::string> {

  //std::cout << " HTTP request > " << http_request;

  std::string http_request_type, requested_file, http_version;
  std::stringstream parser(http_request);
  parser >> http_request_type >> requested_file >> http_version;
  
  std::vector<std::string> http_request_info;
  http_request_info.push_back(http_request_type);
  http_request_info.push_back(requested_file);
  http_request_info.push_back(http_version);
  
  return http_request_info;
}

void display_html(FILE* client_write_fp, std::string resource_path){
    const char *data = "";          
    std::string line;  
    std::ifstream textFile;                        
    textFile.open(resource_path);

    while (getline(textFile,line)){
        //std::cout << line << std::endl;         
        data = line.c_str();
        fprintf(client_write_fp, "%s", data);  
    } 
    textFile.close();
}

//Logs request to stdout or file based on -d flag
auto log_http_request(const http_request_data& queue_block, const server_options& options) -> void{

   //Build log and write to either stdout or options.files
   
   std::string log_output;
   std::string ip_address = queue_block.ip_address;      
   
   std::stringstream log_stream; 
   log_stream << ip_address << " - [" << queue_block.time_received << "] [" << queue_block.time_sent << "] "
              << '"' << queue_block.http_request << "\" " << queue_block.status_code << " " << queue_block.content_length; 
              
   log_output = log_stream.str();

   
   if(options.debug_flag){
     //std::cout << "Writing log to stdout\n";
     std::cout << log_output << std::endl;
   }
   else{
     std::ofstream log_file;
     log_file.open(options.file, std::ios::app);
     log_file << log_output << std::endl;
     log_file.close();
   }
}

auto list_current_directory(const std::string& input) -> std::vector<std::string>{
   
   std::vector<std::string> directory_list;
   
   DIR *dir;
   struct dirent *ent;
   dir = opendir (input.c_str());
   if (dir != NULL) {
     /* print all the files and directories within directory */
     while ((ent = readdir (dir)) != NULL) {
       std::string list_entry = ent->d_name;
       directory_list.push_back(list_entry);       
     }
     closedir (dir);
     sort(directory_list.begin(), directory_list.end());
     
     return directory_list;
   } 
   else {
     /* could not open directory */
     perror ("");
   }
}


// Outputs the resource data to the write stream for web browser rendering
void data_to_client(FILE* client_write_fp, std::string request_type, std::string resource_path, int &content_bytes, int &status_code_int)
{
    // Determine if request is valid and generate proper status code  
    FILE *file_ptr = fopen(resource_path.c_str(), "rb");  
    std::string codes[] = {"200 OK", "404 Not Found", "400 Bad Request"};
    std::string last_modified = get_time();       
    std::string status_code = codes[1]; 
    status_code_int = 404;
    
                                                                                
    if (file_ptr != NULL){
        status_code = codes[0];
        last_modified = file_last_modified(resource_path);   
        status_code_int = 200;                      
    } 
    if (request_type == "HEAD"){
        content_bytes = 0;
    }
     
    // HTTP header creation
    fprintf(client_write_fp, "HTTP/1.1 %s\n", status_code.c_str());  
    fprintf(client_write_fp, "Date: %s\n", get_time().c_str());  
    fprintf(client_write_fp, "Server: Myhttpd 1.0\n");  
    fprintf(client_write_fp, "Last-Modified: %s\n", last_modified.c_str());  
    fprintf(client_write_fp, "Content-Type: %s\n", file_content_type(resource_path).c_str());   
    // fprintf(client_write_fp, "Content-Length: %u", content_bytes);                                                                  
    fprintf(client_write_fp, "\r\n"); 

    // Early return on HEAD requests or path read errors
    if (request_type == "HEAD" || file_ptr == NULL){
        if (file_ptr == NULL){ 
            fprintf(client_write_fp, "The requested resource could not be found");
        }
        return;  
    }

    /* The below sections observe the content type and perform content type specific operations i.e.
       HTML/text files produce formatted string output, image files produce data output after header is sent
       to the client */
    if (file_content_type(resource_path) == "text/html"){
        display_html(client_write_fp, resource_path);     
    }  
    else if (file_content_type(resource_path) == "image/gif" || file_content_type(resource_path) == "image/jpeg") 
    {
        // Write image output data to stream         
        unsigned char buffer[200];    
        FILE *image_fp = fopen(resource_path.c_str(), "rb");
        size_t size;   

        if (image_fp == NULL){
            //std::cout << "Error opening requested file\n" << std::endl;        
            exit(1);
        }       

        while((size = fread(buffer, 1, sizeof(buffer), image_fp)) > 0){
            fwrite(buffer, 1, size, client_write_fp);    
        }   
        fprintf(client_write_fp, "Image file returned. Request file from a web browser to correctly display it.\n");
    }                     
    else {    
            if (is_directory(resource_path))
            {
                if (resource_path.back() != '/'){
                    resource_path += "/"; 
                }
                std::string new_url = resource_path + "index.html";
                file_ptr = fopen(new_url.c_str(), "rb");                          // Check for index.html & try to display

                if (file_ptr != NULL){
                        display_html(client_write_fp, new_url);
                } 
                else {
                    std::vector<std::string> dirs = list_current_directory(resource_path);  // Otherwise show directory listing
                    for (auto &item: dirs){
                        const char* fileName = item.c_str();
                        fprintf(client_write_fp, "%s\n", fileName);
                    }
                }               
            }
    }                   
}       

auto get_exe_path() -> std::string {
    char buffer[PATH_MAX];
    ssize_t buffer_length = readlink( "/proc/self/exe", buffer, PATH_MAX);
    if (buffer_length == -1){
        return std::string("error");
    }
    
    std::string path = buffer;
    std::size_t last_period_position = path.find_last_of('/'); 
    path = path.substr(0, last_period_position);
    
    return buffer;
    
}
