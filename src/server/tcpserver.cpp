#include <iostream>
#include <unistd.h>

#include "server/tcpserver.h"
#include "protocol/protocol.h"

/*
    TCPServer
    :   TCPServer listens on a port and tries to read a packet from the connection fd.
    If it can read a packet, and puts it in a queue. User of the TCPServer can call getNextFd function
    to get file desriptor

    initialize 
        :   initialized and binds the the server socket to desired port
    start 
        :   kicks of the main server thread, and returns immidiatly

    serverLoop 
        :   this is the main server loop, this runs on a thread, and listens for incomming connection
    
    getNextFd
        :   The user of TCPServer can call this to get a file descritor to extract a packet from. Returns -1 if not fd is available

*/

Server::TCPServer::TCPServer(int port,int connLimit,Util::Logger *lg)
{
    m_port=port;
    m_connLimit=connLimit;
    m_logger=lg;

    m_sAddr.sin_family=AF_INET;
    m_sAddr.sin_addr.s_addr=INADDR_ANY;
    m_sAddr.sin_port=htons(m_port);

    m_keepAlive=true;
}

Server::TCPServer::~TCPServer()
{
    close(m_serverFd);
}

int Server::TCPServer::initialize()
{
    /*
    *   Initializes server socket, binds, listen on socket
    *   Returns  0 on success.
    *           -1 on failure to create socket
    *           -2 on failure to bind to socket
    *           -3 on failure to listen on socket
    */
    int retVal;

    m_serverFd=socket(AF_INET,SOCK_STREAM,0);
    if(m_serverFd<0)
        return -1;

    retVal=bind(m_serverFd,(sockaddr*)&m_sAddr,sizeof(m_sAddr));
    if(retVal<0)
        return -2;
    
    retVal=listen(m_serverFd,m_connLimit);
    if(retVal<0)
        return -3;

    return 0;

}

void Server::TCPServer::start()
{
    m_serverLoop=std::thread(Server::TCPServer::serverLoop,this);
}

void Server::TCPServer::terminate()
{
    m_keepAlive=false;
    m_queueMtx.lock();
    m_serverLoop.~thread();
    m_serverLoop.join();
    m_queueMtx.unlock();

}

void Server::TCPServer::serverLoop(Server::TCPServer *self)
{
    while(self->m_keepAlive)
    {
        int cFd=-1;
        cFd = accept(self->m_serverFd,NULL,NULL);
        self->m_logger->trace("TCPServer::serverLoop","Accecpted connection on FD: "+std::to_string(cFd));

        if(cFd<0)
            break;

        self->m_queueMtx.lock();
        self->m_fdQueue.push(cFd);
        self->m_queueMtx.unlock();     
    }
}

int Server::TCPServer::getNextFd()
{
    /*
    *   Returns -1 if queue is empty, else returns a valid FD to socket
    */
    int fd = -1;
    m_queueMtx.lock();
    if(!m_fdQueue.empty())
    {
        fd = m_fdQueue.front();
        m_fdQueue.pop();
    }
    
    m_queueMtx.unlock();
    return fd;
}
