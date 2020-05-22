#include<iostream>
#include<vector>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

#include "protocol.h"

class TCPServer
{
    private:
    int port, serverFd;
    socklen_t addrLen;
    struct sockaddr_in sAddr,cAddr;
    int connLimit;

    typedef struct _h
    {
        TCPServer *self;
        pthread_t *handlerThread;
        int cFd;
    } handler_info;

    std::vector<handler_info*> handlers;
    
    //To make sure you cannot create an object without port and connLimit
    TCPServer();
    
    void joinHandlerThreads()
    {
        for(auto i: handlers)
        {
            pthread_join(*(i->handlerThread),NULL);
        }
    }

    void clearHandlers()
    {
        for(auto i : handlers)
        {
            delete i->handlerThread;
            close(i->cFd);
            delete i;
        }
        handlers.clear();
    }

    public:
    static void* handleIncommingConnetion(void *handlerInfo)
    {
        handler_info *hInfo=(handler_info*)handlerInfo;

        std::string cmdStr=Protocol::readCommandFromNewConnection(hInfo->cFd);
        Protocol::Command *cmd=Protocol::getCommandHandler(cmdStr,hInfo->cFd);
        if(cmd==nullptr)
        {
            std::cout<<"Invalid Packet."<<std::endl;
            send(hInfo->cFd,"Not Ok\0",7,0);
            delete cmd;
            return nullptr;
        }

        // std::cout<<cmdStr<<std::endl;
        cmd->process();
        send(hInfo->cFd,"Ok\0",7,0);
        delete cmd;      

    }

    TCPServer(int port,int connLimit)
    {
        int retVal;

        this->port=port;
        this->connLimit=connLimit;

        sAddr.sin_family=AF_INET;
        sAddr.sin_addr.s_addr=INADDR_ANY;
        sAddr.sin_port=htons(port);

        serverFd=socket(AF_INET,SOCK_STREAM,0);
        if(serverFd<0)
            throw "Unable to create socket.";

        retVal=bind(serverFd,(sockaddr*)&sAddr,sizeof(sAddr));
        if(retVal<0)
            throw "Unable bind to address.";
        
        retVal=listen(serverFd,connLimit);
        if(retVal<0)
            throw "Cannot listen on the socket.";
    }

    void start()
    {
        while(true)
        {
            for(int i=0;i<connLimit;i++)
            {
                addrLen=sizeof(cAddr);
                int cFd;
                cFd=accept(serverFd,(sockaddr*)&cAddr,&addrLen);
                if(cFd<0)
                {
                    std::cout<<"Failed to accpet connection"<<std::endl;
                    break;
                }

                handler_info *hInfo= new handler_info;
                pthread_t *thread=new pthread_t;
                hInfo->cFd=cFd;
                hInfo->self=this;
                hInfo->handlerThread=thread;

                pthread_create(thread,NULL,TCPServer::handleIncommingConnetion,hInfo);
                handlers.push_back(hInfo);
            }
            joinHandlerThreads();
            clearHandlers();
            break;
        }
    }

    ~TCPServer()
    {
        joinHandlerThreads();
        clearHandlers();
    }
};



int main()
{
    TCPServer server(8080,3);
    server.start();
}