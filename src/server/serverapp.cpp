#include<iostream>
#include<thread>
#include<unistd.h>
#include<vector>
#include<cstring>

#include "server/tcpserver.h"
#include "util.h"

int main()
{
    Util::Logger logger("../log/serverapp.log",Util::Logger::TRACE);

    Server::TCPServer server(8080,500,&logger);
    int err=server.initialize();
    if(err<0)
    {
        exit(-1);
    }

    server.start();

    const char *_pl=" Hello from client!Another Payload";
    const char *_md="|somemeta|meta data";
    int count;

    for(int i=0;i<100;i++)
    {
        int fd = server.getNextFd();
        if(fd == -1)
        {
            // std::cout<<"Nothing to read. Sleeping"<<std::endl;
            sleep(1);
            continue;
        }
        else
        {

            Protocol::Packet pkt(fd,&logger);
            pkt.readPacket();
            const char *meta=pkt.getMetadata();
            const char *payload=pkt.getPayload();
            for(int i=0;i<pkt.getMetadataLen();i++)
                std::cout<<meta[i];
            // int cmp_m=strncmp(meta,_md,pkt.getMetadataLen());
            // int cmp_p=strncmp(payload,_pl,pkt.getPayloadLen());
            // std::cout<<cmp_m<<" "<<cmp_p<<std::endl;
                      
        }
    }
    server.terminate();

}    
    