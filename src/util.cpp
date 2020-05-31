#include <math.h>
#include <string>
#include <map>
#include <vector>
#include "util.h"

int lpadIntToStr(int num,char *buffer, int len)
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

int parseMapFromStr(const std::string &str, std::map<std::string,std::string> &kv)
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


int parseStrFromMap(const std::map<std::string,std::string> &kv,std::string &str)
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