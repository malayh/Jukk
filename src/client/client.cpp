#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#include "util.h"
#include "protocol/protocol.h"

class TCPClient
{
    private:
    int port, clientSocket;
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

        clientSocket=socket(AF_INET,SOCK_STREAM,0);
        if(clientSocket<0)
            throw "Unable to create socket.";

    }
    
    void eshtablishConnection()
    {
        int retVal;
        retVal=connect(clientSocket,(sockaddr*)&sAddr,sizeof(sAddr));
        if(retVal<0)
        {
            std::cout<<"Unable to connect to server."<<std::endl;
            exit(-1);
        }
    }

    void test()
    {
        char buffer[9];
        std::string meta="meta data";
        std::string payload="Hello from client!";

        //command
        lpadIntToStr(1,buffer,8);
        send(clientSocket,buffer,8,0);
        
        //metalen
        lpadIntToStr(meta.length(),buffer,8);
        send(clientSocket,buffer,8,0);
        //meta
        send(clientSocket,meta.c_str(),meta.length(),0);

        //paload len
        lpadIntToStr(payload.length(),buffer,8);
        send(clientSocket,buffer,8,0);
        //payload
        send(clientSocket,payload.c_str(),payload.length(),0);

        //end
        lpadIntToStr(Protocol::HEART_BEAT,buffer,8);
        send(clientSocket,buffer,8,0);

        recv(clientSocket,buffer,8,0);
        std::cout<<buffer<<std::endl;
        close(clientSocket);

    }
};


int main()
{
    TCPClient client("127.0.0.1",8080);
    client.eshtablishConnection();
    client.test();
    return 0;
}
