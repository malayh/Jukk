#include<iostream>
#include<thread>
#include<unistd.h>

#include "server/tcpserver.h"

int main()
{
    Server::PacketQueue queue;
    Server::TCPServer server(8080,50,&queue);
    int err=server.initialize();
    if(err<0)
    {
        exit(-1);
    }
    server.start();

    for(int i=0;;i++)
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
            delete pkt;            
        }
    }
    server.terminate();



}    
    