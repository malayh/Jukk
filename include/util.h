#ifndef util_h__
#define util_h__
#include<string>
#include<map>

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

        private:
            Loglevel m_loglevel;
    };
};

#endif