#include<iostream>
#include<thread>
#include<unistd.h>
#include<vector>

#include "server/tcpserver.h"
#include "util.h"

int main()
{
    Server::PacketQueue queue;
    Util::Logger logger("../log/serverapp.log",Util::Logger::TRACE);

    Server::TCPServer server(8080,500,&queue,&logger);
    int err=server.initialize();
    if(err<0)
    {
        exit(-1);
    }
    server.start();

    std::vector<Protocol::Packet*> pkts;

    for(int i=0;i<100;i++)
    {
        if(queue.isEmpty())
        {
            // std::cout<<"Nothing to read. Sleeping"<<std::endl;
            sleep(1);
            continue;
        }
        else
        {
            Protocol::Packet *pkt=queue.pop();
            const char *meta=pkt->getMetadata();
            const char *payload=pkt->getPayload();
            
            for(int i=0;i<pkt->getMetadataLen();i++)
                std::cout<<meta[i];
            std::cout<<std::endl;

            for(int i=0;i<pkt->getPayloadLen();i++)
                std::cout<<payload[i];
            std::cout<<std::endl;
            
            std::cout<<pkt->getConnFd()<<std::endl<<std::endl;
            pkts.push_back(pkt);            
        }
    }
    for(auto i: pkts)
    {
        delete i;
    }
    server.terminate();

}    
    