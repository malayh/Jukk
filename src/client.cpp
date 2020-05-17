#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

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
            throw "Unable to connect to server.";
    }

    void test()
    {
        char buffer[1024];
        send(clientSocket,"Client",6,0);
        read(clientSocket,buffer,1024);
        std::cout<<buffer<<std::endl;
        close(clientSocket);
    }
};
int main(int argc, char const *argv[])
{
    TCPClient client("192.168.43.15",8080);
    client.eshtablishConnection();
    client.test();
    return 0;
}
