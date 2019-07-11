#include <cstdlib>
#include <math.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <net/if.h>
#include <string.h>
#include <arpa/inet.h>
#include<stdio.h>
#include <sys/ioctl.h>
#include "didiutils.h"

static const int GENERIC_DIGITS = 32;

#define PRINT_RETURN(N, format, value) \
    char buffer[N]; \
    ::snprintf(buffer, N, format, value); \
    return buffer; 

std::string DidiUtils::print(int value)
{
    PRINT_RETURN(GENERIC_DIGITS, "%d", value)
}

std::string DidiUtils::print(uint32_t value)
{
    PRINT_RETURN(GENERIC_DIGITS, "%u", value)
}

std::string DidiUtils::print(uint64_t value)
{
    PRINT_RETURN(GENERIC_DIGITS, "%lu", value)
}

std::string DidiUtils::print(double value)
{
    PRINT_RETURN(GENERIC_DIGITS, "%f", value)
}

void DidiUtils::split_str(const string& str, vector<string>& tokens, 
    const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0); 
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);    
    tokens.clear();
    while (string::npos != pos || string::npos != lastPos)
    {   
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }   
}

void DidiUtils::split_str(const string& str, vector<uint64_t>& tokens, 
    const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0); 
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);    
    tokens.clear();
    uint64_t value = 0;
    while (string::npos != pos || string::npos != lastPos)
    {   
        value = strtoul(str.substr(lastPos, pos - lastPos).c_str(),NULL,10);
        // Found a token, add it to the vector.
        tokens.push_back(value);
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }   
}
bool DidiUtils::get_date_time(char *strdate,int strdate_size)
{
    if(strdate == NULL)
    {   
        return false;
    }   
    long t_now = 0;
                
    struct timeval t;
    gettimeofday(&t, NULL);
    t_now = t.tv_sec;
                         
    struct tm vtm;
    struct tm *p_tm = localtime_r(&t_now, &vtm);
    if (!p_tm)
    {   
        return false;
    }   
    snprintf(strdate,strdate_size,"%04d-%02d-%02d %02d:%02d:%02d",vtm.tm_year+1900, vtm.tm_mon + 1, vtm.tm_mday, vtm.tm_hour, vtm.tm_min, vtm.tm_sec);
    return true;
}


uint64_t DidiUtils::time()
{
    return ::time(0);
}

uint64_t DidiUtils::timeus() {
    timeval time;
    ::gettimeofday(&time, 0);
    return time.tv_sec*1000*1000 + time.tv_usec;
}

uint64_t DidiUtils::timems() {
    timeval time;
    ::gettimeofday(&time, 0);
    return time.tv_sec*1000 + time.tv_usec/1000;
}

uint64_t DidiUtils::get_morning_time() {
    time_t t = ::time(0);
    struct tm *time = ::localtime(&t);
    time->tm_hour = 0;
    time->tm_min = 0;
    time->tm_sec = 0;
    t = mktime(time);
    return t;
}

double DidiUtils::timef()
{
    timeval time;
    ::gettimeofday(&time, 0);
    return time.tv_sec + time.tv_usec/1000.0/1000.0;
}

#define inaddrr(x) (*(struct in_addr *) &ifr->x[sizeof sa.sin_port])
#define MAX_INTERFACE_CNT 32
string DidiUtils::local_ip()
{
    string ip;
    ifreq        intfBuf[MAX_INTERFACE_CNT];
    int          sockfd = -1;
    struct in_addr stInaddr;

    struct ifconf      ifc;
    struct ifreq       *ifr;
    struct sockaddr_in sa;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    if (sockfd < 0 )
    {
        return ip;
    }

    // init the ifc struct
    memset(&ifc, 0, sizeof(ifc));
    ifc.ifc_len = sizeof(intfBuf);
    ifc.ifc_req = intfBuf;

    if (0 != ioctl(sockfd, SIOCGIFCONF, &ifc))
    {
        close(sockfd);
        return ip;
    }

    if (sizeof(intfBuf) <= (unsigned int)ifc.ifc_len)
    {
        close(sockfd);
        return ip;
    }

    ifr = ifc.ifc_req;

    for (; (char *) ifr < (char *) ifc.ifc_req + ifc.ifc_len; ++ifr)
    {
        if (ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data)
        {
            continue;  /* duplicate, skip it */
        }

        if (ioctl(sockfd, SIOCGIFFLAGS, ifr))
        {
            continue;  /* failed to get flags, skip it */
        }
       	stInaddr = inaddrr(ifr_addr.sa_data); 
        ip = inet_ntoa(stInaddr);
        if (0==strncmp(ip.c_str(), "172", 3) || 0==strncmp(ip.c_str(), "192", 3) || 0==strncmp(ip.c_str(), "10.", 3))
        {           
            break;
        }
    }

    close (sockfd);

    return ip;
}

/*
 * 注释请见函数声明
 */
long DidiUtils::get_mysql_orderid(long a) {

    //高18位
    long highpart = (a >> 46) & 0x3ffff;

    //低32位
    long lowpart = (a & 0xffffffff);
    return (highpart << 32) + lowpart;
}

/*
 * 把字符串s按字符串c分割，结果存入v中
 */
void DidiUtils::SplitString(const string& s, vector<std::string>& v, const string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

/*
 * 删除字符串中所有的空格
 */
void DidiUtils::string_trim(std::string& s){
    int index = 0;
    if(!s.empty()){
        while((index = s.find(' ', index)) != string::npos)
        {
            s.erase(index, 1);
        }
    }
}

/*
 * 注释请见函数声明
 */
long DidiUtils::get_district_id(long a) {

    //中间14位
    long midpart = (a >> 32) & 0x3fff;
    return midpart;
}

int main_test(){
	string ip ;
	ip = DidiUtils::local_ip();
	//ip = GetLocalIp();
	printf("ip=%s",ip.c_str());
}
