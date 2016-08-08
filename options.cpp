#include <iostream>
#include <string>
#include <limits.h>
#include <unistd.h>
 

#include <structs.h>
#include <functions.h>

void set_default_server_options(server_options& options){

    options.debug_flag = false;
    options.logging_enabled = false;
    options.file = "";
    options.port = 8080;
    options.dir = get_exe_path();
    options.time = 60;
    options.threadnum = 4;
    options.sched = "FCFS";
}

void show_server_info(const server_options& options){
    
    std::cout << "=============" << std::endl;
    std::cout << "Program Info" << std::endl;
    std::cout << "=============" << std::endl;
   
    if(options.debug_flag == true){
        std::cout << "Debug mode: enabled" << std::endl;
        std::cout << "Logging: stdout" << std::endl;
    }
    
    else{
        std::cout << "Debug mode: disabled" << std::endl;
        std::cout << "Logging: ";
        if (options.file == "") std::cout << "no file specified" << std::endl;
        else std::cout << options.file << std::endl;
    }    
    std::cout << "Port: " << options.port << std::endl;
    std::cout << "Directory: " << options.dir << std::endl;
    std::cout << "Queuing time: " << options.time << " seconds" << std::endl;
    std::cout << "No. of threads: " << options.threadnum << std::endl;
    std::cout << "Scheduling policy: " << options.sched << std::endl << std::endl;
       
}

void show_help(){
   
    std::cout << "usage: myhttpd [−d] [−h] [−l file] [−p port] [−r dir] [−t time] [−n threadnum] [−s sched]" << std::endl;
    std::cout << "\t-d: Enter debugging mode." << std::endl;
    std::cout << "\t-h: Show usage summary." << std::endl;
    std::cout << "\t-l file: Log all requests to the given file." << std::endl;
    std::cout << "\t-p port: Listen on the given port. If not provided, myhttpd will listen on port 8080." << std::endl;
    std::cout << "\t-r dir: Set the root directory for the http server to dir." << std::endl;
    std::cout << "\t-t time: Set the queuing time to time seconds. The default should be 60 seconds." << std::endl;
    std::cout << "\t-n threadnum: : Set number of threads waiting ready in the execution thread pool to threadnum." << std::endl;
    std::cout << "\t                The default should be 4 execution threads." << std::endl;
    std::cout << "\t-s sched: Set the scheduling policy. It can be either FCFS or SJF. The default will be FCFS" << std::endl;
}




