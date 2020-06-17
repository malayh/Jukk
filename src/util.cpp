#include <math.h>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <time.h>

#include "util.h"

int  Util::padIntToStr(int num,char *buffer, int len)
{
    /*
    * It will lpad 'num' with 0s till the desired value of 'len'
    * 'buffer' has have enough space to accomodate 'len+1' number of characters, because of \0 at the end 
    */

    if(num>pow(10,len)-1 || num<0)
        return -1;
        
    //this is clever! I like it!
    buffer[len]='\0';
    for(;--len>=0;num/=10)
        buffer[len]='0'+(num%10);

    return 0;
}

int Util::parseMapFromStr(const std::string &str, std::map<std::string,std::string> &kv)
{
    /*
    *   Reads pipe "|" delimited string "str", and parse key value pairs, and add them
    *   to "kv".
    *   It will expect a even number of fields, first one is considered key and the next one is
    *   considered value.
    *   Eg: A string "foo|123|baz|789" will be translated into a map with key-value pair of
    *   {"foo":"123","baz":"789"}
    * 
    *   Returns -1 on failure, 0 On success 
    */
    std::vector<int> offsets;
    for(int i=0;i<str.length();i++)
    {
        if(str[i] == '|' && i-1>=0)
            offsets.push_back(i);
    }
    if(offsets.size()%2==0)
        return -1;

    offsets.push_back(str.length());

    bool isKey=true;
    int curOffset=0;
    std::string key,value;
    for(int i=0;i<offsets.size();i++)
    {
        int l=offsets[i]-curOffset;
        if(isKey)
        {
            key=str.substr(curOffset,l);
            isKey=false;
        }
        else
        {
            value=str.substr(curOffset,l);
            isKey=true;
            kv.insert(std::pair<std::string,std::string>(key,value));
        }

        curOffset=offsets[i]+1;
    }

    return 0;
}

int Util::parseStrFromMap(const std::map<std::string,std::string> &kv,std::string &str)
{
    if(kv.empty())
        return -1;
        
    str="";
    for(auto i=kv.begin();i!=kv.end();i++)
    {
        str.append(i->first);
        str.append("|");
        str.append(i->second);        
    }
    
    return 0;
}

// ------------ Logger ------------
Util::Logger::Logger(const std::string &filepath,Util::Logger::Loglevel loglevel)
{
    m_filePath=new char[filepath.length()+1];
    memcpy(m_filePath,filepath.c_str(),filepath.length());
    m_filePath[filepath.length()]='\0';

    m_logLevel=loglevel;    
    m_logFile.open(m_filePath,std::ios::app);

    m_timestampFormat="%T %d/%m/%y";
}

Util::Logger::~Logger()
{
    delete[] m_filePath;
    m_logFile.close();
}

int Util::Logger::getCurrentTimestamp(char *buffer, int maxsize)
{
    if(maxsize<20)
        return -1;

    time_t t=time(NULL);
    strftime(buffer,maxsize,m_timestampFormat.c_str(),localtime(&t));
    return 0;
}

int Util::Logger::trace(const std::string &moduleName,const std::string &msg)
{
    if(m_logLevel<=Logger::TRACE)
    {
        char time[20];
        getCurrentTimestamp(time,20);
        m_fileMtx.lock();
        m_logFile<<"TRACE: "<<time<<" :["<<moduleName<<"]: "<<msg<<std::endl;
        m_fileMtx.unlock();
    }
}

int Util::Logger::debug(const std::string &moduleName,const std::string &msg)
{
    if(m_logLevel<=Logger::DEBUG)
    {
        char time[20];
        getCurrentTimestamp(time,20);
        m_fileMtx.lock();
        m_logFile<<"DEBUG: "<<time<<" :["<<moduleName<<"]: "<<msg<<std::endl;
        m_fileMtx.unlock();
    }
}

int Util::Logger::info(const std::string &moduleName,const std::string &msg)
{
    if(m_logLevel<=Logger::INFO)
    {
        char time[20];
        getCurrentTimestamp(time,20);
        m_fileMtx.lock();
        m_logFile<<"INFO: "<<time<<" :["<<moduleName<<"]: "<<msg<<std::endl;
        m_fileMtx.unlock();
    }
}

int Util::Logger::warn(const std::string &moduleName,const std::string &msg)
{
    if(m_logLevel<=Logger::WARNING)
    {
        char time[20];
        getCurrentTimestamp(time,20);
        m_fileMtx.lock();
        m_logFile<<"WARN: "<<time<<" :["<<moduleName<<"]: "<<msg<<std::endl;
        m_fileMtx.unlock();
    }
}

int Util::Logger::error(const std::string &moduleName,const std::string &msg)
{
    if(m_logLevel<=Logger::ERROR)
    {
        char time[20];
        getCurrentTimestamp(time,20);
        m_fileMtx.lock();
        m_logFile<<"ERROR: "<<time<<" :["<<moduleName<<"]: "<<msg<<std::endl;
        m_fileMtx.unlock();
    }
}