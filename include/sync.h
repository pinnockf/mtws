#ifndef SYNC_H 
#define SYNC_H

#include <vector>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <structs.h>

extern std::mutex data_mutex, work_mutex, log_mutex;
extern std::condition_variable_any shed_cv, work_cv;
extern server_options options;
extern std::vector<http_request_data> http_request_queue;

#endif
