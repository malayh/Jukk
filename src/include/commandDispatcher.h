#ifndef __command_dispatcher_h
#define __command_dispatcher_h
#include<iostream>
#include<map>
#include "protocol.h"

namespace Protocol
{
    class CommandDispatcher
    {
        private:    
        std::map<std::string,Protocol::Command*> commands;
        std::string readCommand(int fd); 

        public:
        CommandDispatcher();
        int processIncoming(int fd);
    };
}
#endif