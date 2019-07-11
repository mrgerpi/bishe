#include<string>
#include<stdint.h>
#include<vector>
#include <unistd.h>
using namespace std; 

#define TimeConsumeS(begs)    \
    (int)(DidiUtils::time()-begs)

#define TimeConsumeMs(begms)  \
    (((int)(DidiUtils::timems()-begms))<0?0:((int)(DidiUtils::timems()-begms)))

#define TimeConsumeUs(begus)  \
    (int)(DidiUtils::timeus()-begus) 

#ifndef DIDI_COMM_UTILS_H_
#define DIDI_COMM_UTILS_H_

class DidiUtils
{
public:

	//string
	static std::string print(int value);
    static std::string print(uint32_t value);
    static std::string print(uint64_t value);
    static std::string print(double value);
    static void string_trim(std::string& s);
	static void split_str(const string& str, vector<string>& tokens, 
	const string& delimiters);
	static void split_str(const string& str, vector<uint64_t>& tokens, 
    const string& delimiters);

    static bool get_date_time(char *strdate,int strdate_size);
	//time
	static uint64_t time(); // in seconds, diff 1s
	static uint64_t timeus(); // in micro seconds, diff 1us
	static uint64_t timems();
	static double timef(); // in seconds, diff 1us
	static uint64_t get_morning_time();

    /*
     * http://code.xiaojukeji.com/wiki/GulfStream/Zhuan-Che-guidSheng-Ji-Fang-An
     */
    static long get_mysql_orderid(long a);
    static long get_district_id(long a);

	static string local_ip();
	static void SplitString(const string& s, vector<std::string>& v, const string& c);

	static int readFile(const string& path, string& content);


	static int mkdirs(const string& path);
	static int writeFile(const string& path, const string& content);

	static std::string pwd();

private:
    // this class is a helper class which cannot be instantiated
    DidiUtils();
    ~DidiUtils();
};

#endif //DIDI_COMM_UTILS_H_
