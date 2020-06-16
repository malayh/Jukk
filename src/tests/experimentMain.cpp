/*
*      This is where I will will write various experiments to understand certain language features
*/

#include<cstdio>
#include<iostream>
#include<sys/socket.h>
#include<unistd.h>

void readFromFileAsSocket()
{
    FILE *fp=fopen("../resource/exp/packet.txt","r");
    int fd=fileno(fp);

    std::cout<<"FD: "<<fd<<std::endl;

    char buffer[9];
    int err=read(fd,buffer,8);
    std::cout<<"Error code: "<<err<<std::endl;
    if(err>0)
    {
        buffer[8]='\0';
        std::cout<<"Read: "<<buffer<<std::endl;
    }

    fclose(fp);
}

void fileAndLineMacro()
{
    std::cout<<__FILE__<<std::endl;
}

int main()
{
    // readFromFileAsSocket();
    fileAndLineMacro();
    return 0;
}