#include<sys/socket.h>
#include<netinet/in.h>
#include<atomic>


/*
*   This  is the Jukk. This can act both as a server and a client. It will start as a client
*   Plan: If it's running as a server
        -It will a start a TCPSever
        -Incoming packactes will be processed in the main event loop
        -It will keep a stateholder for packets that require a state to be hold
        -The connections fds will be kept alive when the client is acting like a server (?) 
        (The packet destructor closes the fd now, need to change that) 
        -Main event loop:
            -It will take packets comming from main PacketQueue and run a handler for that packet in
            the same thread or a different thread
    Plan: If it's acting a client, ( meaning it is not opening any listening socket)
        -It will initiate the protocol by sending a heartbeat.
        -If the server wants the client to serve something, it will write a packet in the same fd saying so, and
        it will make an entry in the stateholder with a token. That token will be part of the request packet.
        -Then client will read the request packet from server on the same fd, and close the fd.
        -Then it will send the reponse packet with the token it recieved from server so that server.
        -Rest of the communication will happen using the client as an identifier for that particular conversation
*/


class TCPServer_V2
{
        private:
                int m_port, m_serverFd, m_connLimit;
                struct sockaddr_in m_sAddr;
                std::atomic<bool> m_keepAlive;                

                TCPServer_V2();
        public:
                int getNextStreamFd();
                int start();
                int shutdown();
                TCPServer_V2(int,int);
                ~TCPServer_V2();

};
int main()
{

}