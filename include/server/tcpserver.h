#ifndef jukk_server_tcpserver_h
#define jukk_server_tcpserver_h

#include<sys/socket.h>
#include<netinet/in.h>
#include<vector>
#include<thread>
#include<queue>
#include<mutex>
#include<atomic>

#include "protocol/protocol.h"

namespace Server
{
    class PacketQueue
    {
        private:
            std::queue<Protocol::Packet*> m_queue;
            std::mutex m_queueLock;
        public:
            void push(Protocol::Packet*);
            Protocol::Packet* pop();
            bool isEmpty();
            PacketQueue();
    };

    class TCPServer
    {
        private:
            int m_port, m_serverFd, m_connLimit;
            socklen_t m_addrLen;
            struct sockaddr_in m_sAddr,m_cAddr;

            std::atomic<bool> m_keepAlive;
            std::thread m_serverLoop;

            PacketQueue *m_outputQueue;

            TCPServer();
            static void handleIncommingConnetion(int,TCPServer*);
            static void serverLoop(TCPServer*);

        public:
            void start();
            void terminate();
            int initialize();
            TCPServer(int,int,PacketQueue *);
            ~TCPServer();
    };
};

#endif