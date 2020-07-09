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
#include "util.h"

namespace Server
{

    class TCPServer
    {
        private:
            int m_port, m_serverFd, m_connLimit;
            struct sockaddr_in m_sAddr;

            std::atomic<bool> m_keepAlive;
            std::thread m_serverLoop;

            std::mutex m_queueMtx;
            std::queue<int> m_fdQueue;

            Util::Logger *m_logger;

            TCPServer();
            static void serverLoop(TCPServer*);

        public:
            void start();
            void terminate();
            int initialize();
            TCPServer(int,int,Util::Logger*);
            ~TCPServer();
            int getNextFd();
    };
};

#endif