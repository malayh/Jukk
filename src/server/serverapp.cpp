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

    for(int i=0;i<3;)
    {
        if(queue.isEmpty())
        {
            std::cout<<"Nothing to read. Sleeping"<<std::endl;
            sleep(3);
            i++;
            continue;
        }
        else
        {
            Protocol::Packet *pkt=queue.pop();
            const char *buffer=new char[pkt->getMetadataLen()];
            buffer=pkt->getMetadata();
            std::cout<<buffer<<std::endl;
            delete pkt;
            delete[] buffer;            
        }
    }
    server.terminate();



}    
    