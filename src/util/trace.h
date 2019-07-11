#ifndef __TRACE_H__
#define __TRACE_H__

#include <string>

#define CARPOOL_SR_CALLER "carpool_sr"
struct trace_info_t {
    std::string logId;
    std::string caller;
    std::string spanId;
};

#endif //__TRACE_H__
