#include<iostream>
#include<map>
#include<ctime>
#include<unistd.h>
#include<thread>
#include<vector>

#include "util.h"
#include "server/stateholder.h"

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

void protocol_stateholder()
{
    typedef std::map<std::string,std::string> SMap;
    SMap stateMap= {{"key_1","val 1"}, {"someKey","wierd value 2341345"}};
    SMap stateMap_2= {{"someKye","vaasdf"}, {"someKey","valuasdfasdfe 2341345"},{"key_1","new val for key_1"}};

    SMap *smPtr;

    Protocol::Stateholder sh;
    sh.insertOrUpdateState("token_1",stateMap);
    sh.printStateInternals();

    sh.insertOrUpdateState("new_token",stateMap_2);
    sh.printStateInternals();

    stateMap_2["newAddedKey"]="new added data";
    sh.insertOrUpdateState("new_token",stateMap_2);
    sh.printStateInternals();

    sh.deleteState("token_1");
    sh.printStateInternals();


}

void try_timeStuff()
{
    time_t t=time(0);
    std::cout<<t<<std::endl;
    sleep(5);
    std::cout<<t-time(&t)<<std::endl;
}

void try_mapStuff()
{
    std::map<int,int> var={{1,10},{2,20}};
    const std::map<int,int>* const ptr=&var;
    auto i=var.find(1);

    std::cout<<i->second<<std::endl;
}

void threadWorker()
{
    long long sum=0;
    for(long long i=0;i<100000;i++)
        sum+=i;

    // std::cout<<sum<<std::endl;
}

void try_thread()
{
    std::vector<std::thread> threads;
    for(int i=0;i<100000;i++)
        threads.push_back(std::thread(threadWorker));

    for(auto i=threads.begin();i!=threads.end();i++)
        i->join();
}


int main(int argc, char const *argv[])
{
    // ---- Test stuff --------
    // util_parseMapFromStr();
    // util_parseStrFromMap();
    // protocol_stateholder();

    // ---- Try stuff --------
    // try_timeStuff();
    // try_mapStuff();
    // try_thread();
    return 0;
}
