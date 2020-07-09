#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#include "protocol/protocol.h"

class TCPClient
{
    private:
    int port, m_clientSocket;
    struct sockaddr_in sAddr;

    TCPClient(){};

    public:
    TCPClient(const std::string &address,int port)
    {   
        int retVal;
        sAddr.sin_family=AF_INET;
        sAddr.sin_port=htons(port);
        retVal=inet_pton(AF_INET,address.c_str(),&sAddr.sin_addr);
        if(retVal<0)
            throw "Invalid Address.";

        m_clientSocket=socket(AF_INET,SOCK_STREAM,0);
        if(m_clientSocket<0)
            throw "Unable to create socket.";

    }
    
    void eshtablishConnection()
    {
        int retVal;
        retVal=connect(m_clientSocket,(sockaddr*)&sAddr,sizeof(sAddr));
        if(retVal<0)
        {
            std::cout<<"Unable to connect to server."<<std::endl;
            exit(-1);
        }
    }

    void test()
    {
        Protocol::PacketBuffer pBuffer;

        std::string meta="meta data";
        std::string payload=" Hello from client!";

        pBuffer.putPayload(payload.c_str(),payload.length());
        pBuffer.putMetadata("|somemeta|");
        pBuffer.putMetadata(meta);
        pBuffer.putPayload("Another Payload");
        pBuffer.setPacketType(Protocol::HEART_BEAT);
        pBuffer.sendPacketOnFd(m_clientSocket);
        pBuffer.putMetadata("|sasdf|");
        int err=pBuffer.sendPacketOnFd(m_clientSocket);

        std::cout<<err<<std::endl;
        
        close(m_clientSocket);
        // eshtablishConnection();
        

    }
};


int main()
{
    TCPClient client("127.0.0.1",8080);
    client.eshtablishConnection();
    client.test();
    return 0;
}
