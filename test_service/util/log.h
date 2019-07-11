#ifndef   _MACRO_LogModule
#define   _MACRO_LogModule
#include <stdio.h>
#include <pthread.h>
#include "macro_define.h"
#include <stdint.h>
#include <string>
#include "trace.h"
using std::string;


/* ÿ���̵߳�buffer size*/
//#define   _LOG_BUFFSIZE  1024*1024*4
#define   _LOG_BUFFSIZE  1024*1024*4
/* ��ǰ���̵� Stream IO buffer size*/
#define   _SYS_BUFFSIZE  1024*1024*8
/* log �ļ��ַ���·����󳤶�*/
#define	  _LOG_PATH_LEN  250
/* ��־��Ӧ��ģ����*/
#define   _LOG_MODULE_LEN 32

typedef  enum LogLevel {  
	LL_DEBUG = 1,
	LL_TRACE = 2,
	LL_NOTICE = 3, 
	LL_WARNING = 4, 
	LL_ERROR = 5,
    LL_PUBLIC = 6,
}LogLevel;

// ??��??
class RWLock
{
public:
	explicit RWLock(const pthread_rwlockattr_t* attr = NULL)
	{
		::pthread_rwlock_init(&m_lv, attr);
	}
	~RWLock() { 
		::pthread_rwlock_destroy(&m_lv);
	}
	int RLock(){
		return pthread_rwlock_rdlock(&m_lv);
	}
	int WLock(){
		return pthread_rwlock_wrlock(&m_lv);

	}
	int UnLock(){
		return pthread_rwlock_unlock(&m_lv);
	}

private:
	pthread_rwlock_t m_lv;
	
};


/**
*	Log_Writer  ��־��
*/
class Log_Writer
{
	public:
		Log_Writer()
		{
			m_system_level = LL_NOTICE;
			//fp = stderr;
			fp = NULL;
			m_issync = false;
			m_isappend = true;
			m_filelocation[0] ='\0';
			m_node = 0;
			//pthread_mutex_init(&m_mutex, NULL);
		}
		~Log_Writer(){
			logclose();
		}
		bool loginit(LogLevel l, const  char *filelocation, bool append = true, bool issync = false);
		bool log(LogLevel l,
                 const char *file,
                 int line,
                 const char* function,
                 const char* logformat, ...);
		LogLevel get_level();
		bool logclose();
	private:
		const char* logLevelToString(LogLevel l);
		bool checklevel(LogLevel l);
		int premakestr(char* m_buffer, LogLevel l);
		uint64_t file_node();
		bool _write( char *_pbuffer, int len);
	private:
		enum LogLevel m_system_level;
		FILE* fp;
		bool m_issync;
		bool m_isappend;
		char m_filelocation[_LOG_PATH_LEN];
		//pthread_mutex_t m_mutex;
		RWLock m_lock;
		static __thread char m_buffer[_LOG_BUFFSIZE];
		uint64_t m_node;
		//The __thread specifier may be applied to any global, file-scoped static, function-scoped static, 
		//or static data member of a class. It may not be applied to block-scoped automatic or non-static data member
		//in the log  scence,It's safe!!!!
		//һ���Ա�֮���˳������õ���__thread������Դleak,ͬʱҲ���õ��Ķ��Log_Writer����ţ�
		//��Ϊһ���߳�ͬһʱ��ֻ��һ��Log_Writer�ڸɻ����֮��m_buffer��reset��
		//���Լ���һ���߳��û����Log_Write����(��Ϊһ���߳��ڵ�����ֻ̬�д���) Ҳ���̰߳�ȫ�ģ�����
};

extern Log_Writer PUBLIC_W;
extern Log_Writer WARN_W;
extern Log_Writer INFO_W;
extern Log_Writer CRASH_W;

void set_uuid(const string& log_id = "");
void set_pid(int pid);
const char* get_uuid();
void set_span_id(const string& span_id);
void set_span_id();
const char* get_span_id();
string generateSpanId();
uint64_t generateNumberSpanId();


/**
 * LogLevel ��־����
 * p_modulename ģ���� ��mysql
 * p_logdir  ��־���Ŀ¼
 * */
bool log_init(LogLevel l, const char* p_modulename, const char* p_logdir);
bool ut_log_init(LogLevel l, const char* p_modulename, const char* p_logdir);
bool log_close();
#endif
