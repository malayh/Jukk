#include <iostream>
#include <unistd.h>

#include "server/tcpserver.h"
#include "protocol/protocol.h"

/*
    TCPServer
    :   TCPServer listens on a port and tries to read a packet from the connection fd.
    If it can read a packet, TCPServer puts it to a PacketQueue that can be consumed by the users of TCP Server.

    initialize 
        :   initialized and binds the the server socket to desired port
    start 
        :   kicks of the main server thread, and returns immidiatly

    serverLoop 
        :   this is the main server loop, this runs on a thread, and listens for incomming connection

    handleIncommingConnetion 
        :   serverLoop creates a thread with this function, this function reads the packet from incomming
        connection and put to the queue.
*/

Server::TCPServer::TCPServer(int port,int connLimit,Server::PacketQueue *pQ)
{
    m_port=port;
    m_connLimit=connLimit;
    m_outputQueue=pQ;

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
    m_serverLoop.join();

}

void Server::TCPServer::handleIncommingConnetion(int fd,Server::TCPServer *self)
{
    Protocol::Packet *packet = new Protocol::Packet(fd);

    int err=packet->readPacket();

    if(err==-1)
    {
        delete packet;
        return;
    }

    self->m_outputQueue->push(packet);

}

void Server::TCPServer::serverLoop(Server::TCPServer *self)
{
    while(self->m_keepAlive)
    {
        self->m_addrLen=sizeof(m_cAddr);
        int cFd=accept(self->m_serverFd,(sockaddr*)&self->m_cAddr,&self->m_addrLen);
        if(cFd<0)
            break;
        std::thread _t(TCPServer::handleIncommingConnetion,cFd,self);
        _t.detach();        
    }
}







Server::PacketQueue::PacketQueue(){}

void Server::PacketQueue::push(Protocol::Packet *pkt)
{
    m_queueLock.lock();
    m_queue.push(pkt);
    m_queueLock.unlock();
}

Protocol::Packet* Server::PacketQueue::pop()
{
    Protocol::Packet *pkt;

    m_queueLock.lock();
    if(m_queue.empty())
        pkt=nullptr;
    else
    {
        pkt=m_queue.front();
        m_queue.pop();
    }
    m_queueLock.unlock();

    return pkt;
}

bool Server::PacketQueue::isEmpty()
{
    bool ret;
    m_queueLock.lock();
    ret=m_queue.empty();
    m_queueLock.unlock();
    return ret;
}