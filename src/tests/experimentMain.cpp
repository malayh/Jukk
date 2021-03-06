/*
*      This is where I will will write various experiments to understand certain language features
*/

#include<cstdio>
#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<time.h>
#include<fstream>
#include<string.h>

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

int getTimestamp(char *buffer, int maxsize)
{
    if(maxsize<20)
        return -1;

    time_t t=time(NULL);
    strftime(buffer,maxsize,"%T %d/%m/%y",localtime(&t));
    return 0;
}
void timeStamp()
{
    char buffer[20];
    getTimestamp(buffer,20);
   
    std::cout<<buffer<<std::endl;
}

struct exp_New
{
    int n;
    char *arr;
};
void serializeStruct()
{
    const char *s="asdfagsdflfiugaskdffadfgjkawf";
    struct exp_New arr;
    arr.arr=new char[strlen(s)];
    memcpy(arr.arr,s,strlen(s));
    arr.n=strlen(s);

    std::fstream fbin("../resource/exp/struct.bin",std::ios::out|std::ios::binary);
    fbin.write((char*)&arr,sizeof(arr));
    fbin.close();

}

int main()
{
    // readFromFileAsSocket();
    // fileAndLineMacro();
    // timeStamp();
    serializeStruct();
    return 0;
}