#ifndef util_h__
#define util_h__
#include<string>
#include<map>


int lpadIntToStr(int,char*,int);
int parseMapFromStr(const std::string &, std::map<std::string,std::string> &);
int parseStrFromMap(const std::map<std::string,std::string> &,std::string &);

#endif