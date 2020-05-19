#ifndef command_dispatcher_h
#define command_dispatcher_h
#include<iostream>
#include<map>
#include "protocol.h"

namespace Protocol
{
    class CommandDispatcher
    {
        private:
        int cFD;
        
        std::map<std::string,Protocol::Command*> commands;

    };
}
#endif