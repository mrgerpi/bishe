#ifndef SIMPLE_LOG_H
#define SIMPLE_LOG_H

#include <cstring>
#include <string>
#include <fstream>
#include "sys/time.h"
#include <pthread.h>

const int ERROR_LEVEL = 1;
const int WARN_LEVEL = 2;
const int INFO_LEVEL = 3;
const int DEBUG_LEVEL = 4;

// log config
extern int log_level;

#define log_error(format, args...) \
    if(log_level >= ERROR_LEVEL) { \
		error("%s %s(%d): " format, "ERROR", __FILE__, __LINE__, ##args); \
    }

#define log_warn(format, args...) \
    if(log_level >= WARN_LEVEL) { \
		warn("%s %s(%d): " format, "WARN", __FILE__, __LINE__, ##args); \
    }

#define log_info(format, args...) \
    if(log_level >= INFO_LEVEL) { \
		info("%s %s(%d): " format, "INFO", __FILE__, __LINE__, ##args); \
    }

#define log_debug(format, args...) \
    if(log_level >= DEBUG_LEVEL) { \
		debug("%s %s(%d): " format, "DEBUG", __FILE__, __LINE__, ##args); \
    }


std::string _get_show_time();

int log_init(std::string dir, std::string file);
void error(const char *format, ...);
void warn(const char *format, ...);
void info(const char *format, ...);
void debug(const char *format, ...);
void set_log_level(const char *level);

class FileAppender {
    public:
        FileAppender();
        ~FileAppender();
        int init(std::string dir, std::string file);
        bool is_inited();
        int write_log(char *log, const char *format, va_list ap);
        int shift_file_if_need(struct timeval tv, struct timezone tz);
        int delete_old_log(timeval tv);
        void set_retain_day(int rd);
    private:
        std::fstream _fs;
        std::string _log_file;
        std::string _log_dir;
        std::string _log_file_path;
        long _last_sec;
        bool _is_inited;
        int _retain_day;
        pthread_mutex_t writelock;
};

#endif
