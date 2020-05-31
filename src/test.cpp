#include<iostream>
#include<map>

#include "util.h"

void util_parseMapFromStr()
{
    std::string input_1("key_1|value_1|someKey|val");
    std::map<std::string,std::string> dict;

    parseMapFromStr(input_1,dict);
    auto i=dict.find("key_1");
    if(i!=dict.end())
        std::cout<<"Key: "<<i->first<<" Value: "<<i->second<<std::endl;

}

void util_parseStrFromMap()
{
    std::map<std::string,std::string> dict;
    dict.insert(std::pair<std::string,std::string>("key1","OneVal"));
    dict.insert(std::pair<std::string,std::string>("someKey","some Val"));

    std::string str;
    parseStrFromMap(dict,str);
    std::cout<<str<<std::endl;

}

int main(int argc, char const *argv[])
{
    // util_parseMapFromStr();
    util_parseStrFromMap();
    return 0;
}
