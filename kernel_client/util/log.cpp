#include "log.h"
#include <sys/file.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "uuid/uuid.h"
#include <stdint.h>
#include <sstream>
using std::stringstream;

Log_Writer PUBLIC_W;
Log_Writer WARN_W;
Log_Writer INFO_W;
Log_Writer CRASH_W;
__thread char Log_Writer::m_buffer[_LOG_BUFFSIZE];
static const int LOGID_MAX_LENGTH = 65;
static __thread char g_uuid[LOGID_MAX_LENGTH];
static __thread int g_pid = 0;
//static __thread uint64_t g_span_id;
static __thread char g_span_id[LOGID_MAX_LENGTH];
static uint64_t g_span_id_atomic = 0;

void set_pid(int pid)
{
    g_pid = pid;  
}

void set_uuid(const string& log_id)
{
    if (log_id.empty())
    {
        //uuid_t src_uid;    
        //uuid_generate(src_uid ); //有性能问题
        //uint64_t* p_uid = (uint64_t*)src_uid;
        //uint64_t* p_uid2 = (uint64_t*)src_uid + 1;
        //snprintf(g_uuid, LOGID_MAX_LENGTH,  "%lx%lx", p_uid[0],p_uid2[0]);
        uint64_t src_uid = __sync_fetch_and_add(&g_span_id_atomic, 1);
        snprintf(g_uuid, LOGID_MAX_LENGTH,  "%llx", src_uid);
    }
    else
    {
        strncpy(g_uuid, log_id.c_str(), LOGID_MAX_LENGTH-1);
    }    
    g_uuid[LOGID_MAX_LENGTH-1] = 0x0;
}

const char* get_uuid()
{
    return g_uuid;
}

string generateSpanId()
{
    //stringstream buf;
    //uuid_t src_uid;    
    //uuid_generate(src_uid); //有性能问题
    //uint64_t* p_uid = (uint64_t*)src_uid;
    //buf << p_uid[0];
    //return buf.str();
    char span_id[128];
    int len = snprintf(span_id, 128, "%llx", __sync_fetch_and_add(&g_span_id_atomic, 1));
    span_id[len] = 0x0;
    return std::string(span_id);
}

uint64_t generateNumberSpanId()
{
    //uuid_t src_uid;
    //uuid_generate(src_uid); //有性能问题
    //uint64_t* p_uid = (uint64_t*)src_uid;
    //return p_uid[0];
    uint64_t src_uid = __sync_fetch_and_add(&g_span_id_atomic, 1);
    return src_uid;
}

void set_span_id(const string& span_id)
{
    strncpy(g_span_id, span_id.c_str(), LOGID_MAX_LENGTH-1);
    g_span_id[LOGID_MAX_LENGTH-1] = 0x0;
}
void set_span_id()
{
    set_span_id(generateSpanId());
}

const char* get_span_id()
{
    return g_span_id;
}

bool log_init(LogLevel l, const char* p_modulename, const char* p_logdir)
{
    char _location_str[_LOG_PATH_LEN];  

    snprintf(_location_str, _LOG_PATH_LEN, "%s/crash.log", p_logdir);    
    CRASH_W.loginit(LL_WARNING, _location_str);

    snprintf(_location_str, _LOG_PATH_LEN, "%s/public.log", p_logdir);    
    PUBLIC_W.loginit(l, _location_str);
        
    snprintf(_location_str, _LOG_PATH_LEN, "%s/%s_access.log", p_logdir, p_modulename);
    INFO_W.loginit(l, _location_str);
    
    snprintf(_location_str, _LOG_PATH_LEN, "%s/%s_error.log", p_logdir, p_modulename);    
    
    if(l > LL_WARNING)
        WARN_W.loginit(l, _location_str);
    else
        WARN_W.loginit(LL_WARNING, _location_str);
    return true;
}

bool ut_log_init(LogLevel l, const char* p_modulename, const char* p_logdir)
{
    char _location_str[_LOG_PATH_LEN];

    snprintf(_location_str, _LOG_PATH_LEN, "%s/%s_access.log", p_logdir, p_modulename);
    INFO_W.loginit(l, _location_str);

    snprintf(_location_str, _LOG_PATH_LEN, "%s/%s_error.log", p_logdir, p_modulename);

    if(l > LL_WARNING)
        WARN_W.loginit(l, _location_str);
    else
        WARN_W.loginit(LL_WARNING, _location_str);
    return true;
}

bool log_close(){
    PUBLIC_W.logclose();
    WARN_W.logclose();
    INFO_W.logclose();
    CRASH_W.logclose();
    return true;
}

const char* Log_Writer::logLevelToString(LogLevel l) {
    switch ( l ) {
        case LL_DEBUG:
            return "DEBUG";
        case LL_TRACE:
            return "TRACE";
        case LL_NOTICE:
            return "NOTICE";
        case LL_WARNING:
            return "WARN" ;
        case LL_ERROR:
            return "ERROR";
        case LL_PUBLIC:
            return "PUBLIC";        
        default:
            return "UNKNOWN";
    }
}

bool Log_Writer::checklevel(LogLevel l)
{
    if(l >= m_system_level)
        return true;
    else
        return false;
}

bool Log_Writer::loginit(LogLevel l, const  char *filelocation, bool append, bool issync)
{
    MACRO_RET(NULL != fp, false);
    m_system_level = l;
    m_isappend = append; 
    m_issync = issync; 
    if(strlen(filelocation) >= (sizeof(m_filelocation) -1))
    {
        fprintf(stderr, "the path of log file is too long:%d limit:%d\n", strlen(filelocation), sizeof(m_filelocation) -1);
        exit(0);
    }
    //本地存储filelocation  以防止在栈上的非法调用调用
    strncpy(m_filelocation, filelocation, sizeof(m_filelocation));
    m_filelocation[sizeof(m_filelocation) -1] = '\0';

    if('\0' == m_filelocation[0])
    {
        fp = stdout;
        fprintf(stderr, "now all the running-information are going to put to stderr\n");
        return true;
    }

    fp = fopen(m_filelocation, append ? "a":"w");
    if(fp == NULL)
    {
        fprintf(stderr, "%d cannot open log file,file location is %s\n", g_pid, m_filelocation);
        exit(0);
    }
    
    m_node = file_node();
    
    //setvbuf (fp, io_cached_buf, _IOLBF, sizeof(io_cached_buf)); //buf set _IONBF  _IOLBF  _IOFBF
    setvbuf (fp,  (char *)NULL, _IOLBF, 0);
    fprintf(stderr, "%d now all the running-information are going to the file %s\n", g_pid, m_filelocation);
    return true;
}

int Log_Writer::premakestr(char* m_buffer, LogLevel l)
{
    if (l == LL_PUBLIC) {
        return 0;
    }
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);
    struct tm vtm;
    localtime_r(&tv.tv_sec,&vtm);
    return snprintf(m_buffer, _LOG_BUFFSIZE, "%s: %04d-%02d-%02d %02d:%02d:%02d.%06d [%d:%lx] ",
        logLevelToString(l),
        vtm.tm_year+1900, vtm.tm_mon + 1, vtm.tm_mday, 
        vtm.tm_hour, vtm.tm_min, vtm.tm_sec, tv.tv_usec/1000,
        g_pid, g_span_id);
}


bool Log_Writer::log(LogLevel l,
                     const char *file,
                     int line,
                     const char* function, 
                     const char* logformat, ...)
{
    MACRO_RET(!checklevel(l), false);
    int _size;
    int prestrlen = 0;

    char * star = m_buffer;
    prestrlen = premakestr(star, l);
    star += prestrlen;

    int str_len = 0; 
    if (l!=LL_PUBLIC) {
        str_len = snprintf(star, _LOG_BUFFSIZE-prestrlen, "[%s:%d:%s] ", file, line, function);
    }
    star += str_len;
    
    va_list args;
    va_start(args, logformat);
    _size = vsnprintf(star, _LOG_BUFFSIZE - prestrlen, logformat, args);
    va_end(args);
    star += _size;

    int str_len1 = 0; 
    if (l!=LL_PUBLIC) {
        str_len1 = snprintf(star, _LOG_BUFFSIZE - prestrlen - str_len - _size,
                "||traceid=%s||spanid=%lx\n",
                g_uuid, g_span_id);
    } else {
        str_len1 = snprintf(star, _LOG_BUFFSIZE - prestrlen - str_len - _size, "\n"); 
        //public日志，字段位置不能调整；如果放在最后，日志中就没法增加字段

    }

    if (NULL == fp) {
        fprintf(stderr, "xxxxxxxxxx %d:%s\n", g_pid, m_buffer);
    }else
        _write(m_buffer, prestrlen + str_len + _size + str_len1);
    return true;
}

bool Log_Writer::_write(char *_pbuffer, int len)
{
    if( m_node != file_node() || !m_node ){
        m_lock.WLock();		
		if( m_node != file_node() || !m_node ){
            fprintf(stderr, "xxxxxxxxxx %d:log reinit init file %s\n", g_pid, m_filelocation);
			logclose();
            loginit(m_system_level, m_filelocation, m_isappend, m_issync);
		}
        m_lock.UnLock();
	}

    m_lock.RLock();
    if(1 == fwrite(_pbuffer, len, 1, fp)) //only write 1 item
    {
        if(m_issync)
            fflush(fp);
        *_pbuffer='\0';
    }
    else 
    {
        int x = errno;
        fprintf(stderr, "Failed to write to logfile. errno:%s    message:%s", strerror(x), _pbuffer);
        //return false;
    }
    m_lock.UnLock();
    return true;
}

LogLevel Log_Writer::get_level()
{
    return m_system_level; 
}

bool Log_Writer::logclose()
{
    if(fp == NULL)
        return false;
    fflush(fp);
    fclose(fp);
    fp = NULL;
    m_node = 0;
    return true;
}

uint64_t Log_Writer::file_node()
{	
	uint64_t node = 0;
	struct stat statbuff;	
	if(stat(m_filelocation, &statbuff) == 0 )
	{			
		node = statbuff.st_ino;
	}	
	
	return node;
}


