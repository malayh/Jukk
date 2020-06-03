#include<ctime>
#include<iostream> 

#include "protocol/stateholder.h"

Protocol::Stateholder::Stateholder() : m_expiryTimeout_sec(20) 
{
    //seting timeout to 20 seconds
}

Protocol::Stateholder::~Stateholder()
{
    for(auto i=m_states.begin();i!=m_states.end();i++)
        delete i->second;
}

const std::map<std::string,std::string>* const Protocol::Stateholder::getState(const std::string &token)
{
    /*
    * For a given "token" return a const ptr to a const map of <key,value>
    * Return nullptr if key not found.
    */
    auto i=m_states.find(token);
    if(i==m_states.end())
        return nullptr;
    else
        return i->second;
}

int Protocol::Stateholder::insertOrUpdateState(std::string token, std::map<std::string,std::string> &stateMap)
{
    /* m_states is of structure { "token" : { "key_1":"val_1", "key_2":"val_2" } }
    *  m_stateTimestamps is of structure { "token" : timestamp }
    * 
    * "token" is a access token against which one "state" is stored.
    * every "state" is a dictionary of key-value pairs
    * 
    * For a given "token", this function will look if the "token" was seen before
    *   If yes, then it will update the "state", by copying k-v pairs from "stateMap". 
    *   It will overwrite the "value" if a "key" is alreay present
    * At the end it will insert or update the timestamp for when the state was inserted/modified
    */

    std::map<std::string,std::string> *newState;

    auto found=m_states.find(token);
    if(found==m_states.end())
    {
        newState=new std::map<std::string,std::string>();
        m_states.insert(std::pair<std::string,std::map<std::string,std::string>*>(token,newState));
    }
    else   
        newState=found->second;

    for(auto i=stateMap.begin();i!=stateMap.end();i++)        
        (*newState)[i->first]=i->second;

    insertOrUpdateNowTimestamp(token);
    return 0;   
}

int Protocol::Stateholder::insertOrUpdateNowTimestamp(const std::string &token)
{
    m_stateTimestamps[token]=time(0);
    return 0;
}

int Protocol::Stateholder::deleteTimestamp(const std::string &token)
{
    return m_stateTimestamps.erase(token);
}

int Protocol::Stateholder::deleteState(const std::string &token)
{
    auto found=m_states.find(token);
    if(found==m_states.end())
        return -1;

    delete found->second;
    deleteTimestamp(token);

    return m_states.erase(token);
}

void Protocol::Stateholder::printStateInternals()
{
    std::cout<<"-----------------------------"<<std::endl;
    for(auto i=m_states.begin();i!=m_states.end();i++)
    {
        std::cout<<i->first<<std::endl;
        for(auto j=i->second->begin();j!=i->second->end();j++)
        {
            std::cout<<"\t ("<<j->first<<","<<j->second<<")"<<std::endl;
        }
        std::cout<<"\t Timestamp: "<<m_stateTimestamps[i->first]<<std::endl<<std::endl;
    }
}