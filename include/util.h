#ifndef util_h__
#define util_h__
#include<string>
#include<map>
#include<fstream>
#include<mutex>

namespace Util
{
    int padIntToStr(int,char*,int);
    int parseMapFromStr(const std::string &, std::map<std::string,std::string> &);
    int parseStrFromMap(const std::map<std::string,std::string> &,std::string &);

    class Logger
    {

        public:
            enum Loglevel
            {
                TRACE=0, DEBUG=10, INFO=20, WARNING=30, ERROR=40
            };

            Logger(const std::string&,Loglevel);
            ~Logger();
            int trace(const std::string&,const std::string&);
            int debug(const std::string&,const std::string&);
            int info(const std::string&,const std::string&);
            int warn(const std::string&,const std::string&);
            int error(const std::string&,const std::string&);
            

        private:
            Loglevel m_logLevel;
            char *m_filePath;
            std::ofstream m_logFile;
            std::string m_timestampFormat;
            std::mutex m_fileMtx;

            int getCurrentTimestamp(char *,int);

    };
};

#endif