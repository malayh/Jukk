#pragma once

#include <thread>
#include <vector>

#include "util.h"

void test_Logger_thread(Util::Logger *logger,int n)
{
    switch (n%5)
    {
        case 0:
            logger->trace("test_Logger","Testing: #"+std::to_string(n));
            break;
        case 1:
            logger->debug("test_Logger","Testing: #"+std::to_string(n));
            break;
        case 2:
            logger->info("test_Logger","Testing: #"+std::to_string(n));
            break;
        case 3:
            logger->warn("test_Logger","Testing: #"+std::to_string(n));
            break;
        case 4:
            logger->error("test_Logger","Testing: #"+std::to_string(n));
            break;
    }
}

void test_Logger()
{
    Util::Logger logger("logtest.txt",Util::Logger::TRACE);

    std::vector<std::thread*> threads;
    threads.reserve(50);

    for(int i=0;i<50;i++)
    {
        std::thread *t= new std::thread(test_Logger_thread,&logger,i);
        threads.push_back(t);
    }

    for(auto i: threads)
    {
        i->join();
        delete i;
    }
    
}