#include<iostream>
#include<sys/socket.h>
#include<string.h>

#include "protocol/protocol.h"


std::string Protocol::readCommandFromNewConnection(int cFd)
{
    /*
    * 'cFd' is a socket FD got from accpeting a connection from a server socket
    * This function will consume "8 bytes" of data from 'cFd', parse it into sting and return it
    * Returns empty string if some error occres in reading from socket FD or if the lenght of read string<8
    */

    char buffer[8];
    int err=recv(cFd,buffer,8,0);
    if(err==-1 || strlen(buffer)<8)
        return "";

    return std::string(buffer);

}

Protocol::Command* Protocol::getCommandHandler(std::string cmdStr,int fd)
{
    /*
    * This will take incoming command string and a fd, and return a pointer to a command object
    * Returns:
    *   If cmdStr.len==8 and cmdStr can be converted to Protocol::PacketType 
    *   and cmdStr is a valid PacketType returns an Command object
    *   Otherwise returns nullptr
    * 
    * Note:
    *   Command objects will be heap allocated, call delete when done using them
    */
    if(cmdStr.length()<8)
        return nullptr;

    int pType;
    pType=atoi(cmdStr.c_str());

    switch(pType)
    {
        case Protocol::HEART_BEAT:
            return new Protocol::Heartbeat(fd);
        default:
            return nullptr;
    }
}

// ------- Command -------------------
Protocol::Command::Command(){}
Protocol::Command::Command(int fd){
    connFd=fd;
}
Protocol::Command::~Command(){}
int Protocol::Command::readMetaData(){
    /*
    * set metaData variable if
    *   -The 8 Bytes of the fd can be parsed to a long (say  X)
    *   -and reading on connFd yields len specified by X
    * 
    * sets metaData="" on failure 
    * Return:
    *   -1 on failure
    */

    char metaLen[9];
    char *buffer;
    int len;
    int retVal;

    metaData="";

    retVal=recv(connFd,metaLen,8,0);
    if(retVal<8)
        return -1;

    metaLen[8]='\0';
    len=atoi(metaLen);

    //If we recieve a string that cannot be parser to number, atol will
    //Return 0. The following is to make sure that the sting actually contains a
    //string of 8 zeros in case of len==0. If it contains some other char, then
    //consider that the meta corruped and return -1
    //if not corrupted and len==0, return 0;
    if(len==0)
    {
        for(int i=0;i<8;i++)
            if(metaLen[i]!='0')
                return -1;
        
        return 0;
    }
    
    buffer=new char[len+1];
    retVal=recv(connFd,buffer,len,0);
    if(retVal<len)
        return -1;

    buffer[len]='\0';
    metaData=std::string(buffer);

    delete[] buffer;

    return len;
}
void Protocol::Command::setConnFd(int fd)
{
    connFd=fd;
}

// ------- Hearbeat ------------------
Protocol::Heartbeat::Heartbeat(){}
Protocol::Heartbeat::Heartbeat(int fd): Command(fd){}
Protocol::Heartbeat::~Heartbeat(){}
int Protocol::Heartbeat::processPayload()
{
    /*
    * Works similar to readMetaData
    * Consumes the last 8 bytes of command as well
    * Return -1 on failure
    */
    char payloadLen[9];
    char *buffer;
    int len;
    int retVal;

    retVal=recv(connFd,payloadLen,8,0);
    if(retVal<8)
        return -1;

    payloadLen[8]='\0';
    len=atoi(payloadLen);

    if(len==0)
    {
        for(int i=0;i<8;i++)
            if(payloadLen[i]!='0')
                return -1;
        

        std::cout<<"No payload in heartbeat."<<std::endl;
        return 0;
    }

    buffer=new char[len+1];
    retVal=recv(connFd,buffer,len,0);
    if(retVal<len)
        return -1;

    buffer[len]='\0';
    std::cout<<"Heartbeat Payload:"<<buffer<<std::endl;

    //eating out the last 8 byes of the packet
    retVal=recv(connFd,buffer,8,0);

    delete[] buffer;
    return len;

}
int Protocol::Heartbeat::process()
{
    int retVal=readMetaData();
    if(retVal<0)
        return -1;

    std::cout<<"Payload metadata:"<<metaData<<std::endl;
    retVal=processPayload();
    if(retVal<0)
        return -1;
        
    return 0;
}
bool Protocol::Heartbeat::isStateLess(){ return false; }

//------- Packet -----------------------
Protocol::Packet::Packet(){}
Protocol::Packet::~Packet()
{
    if(m_metadataLen>0)
        delete[] m_metadata;
    if(m_payloadLen>0)
        delete[] m_payload;
}
Protocol::Packet::Packet(int fd)
{
    m_connFd=fd;
    m_packetType=-1;
    m_payloadLen=-1;
    m_metadataLen=-1;
}
int Protocol::Packet::readPacketType()
{
    /*
    * 'm_connFd' is a socket FD got from accpeting a connection from a server socket
    * This function will consume "8 bytes" of data from 'm_connFd'
    * sets m_packetType
    * Returns -1 if error, 0 otherwise
    * 
    */

    char buffer[9];
    int err=recv(m_connFd,buffer,8,0);

    if( err<8 ) 
        return -1;
    
    buffer[8]='\0';
    int packet=atoi(buffer);
    if(packet==0)
    {
        for(int i=0;i<8;i++)
            if(buffer[i]!='0')
                return -1;
    }

    m_packetType=packet;
    return 0;    
}
int Protocol::Packet::readMetadata()
{
    /*
    * set m_metadata and m_metadataLen variable if
    *   -The 8 Bytes of the m_connFd can be parsed to a int (say  X)
    *   -and reading on m_connFd yields len specified by X
    *
    * Return:
    *   -1 on failure, lenght of metadata read othewise
    */

    char buffer[9];

    int err=recv(m_connFd,buffer,8,0);
    if(err<8)
        return -1;

    buffer[8]='\0';
    m_metadataLen=atoi(buffer);

    //If we recieve a string that cannot be parser to number, atol will
    //Return 0. The following is to make sure that the sting actually contains a
    //string of 8 zeros in case of len==0. If it contains some other char, then
    //consider that the meta corruped and return -1
    //if not corrupted and len==0, return 0;
    if(m_metadataLen==0)
    {
        for(int i=0;i<8;i++)
            if(buffer[i]!='0')
                return -1;

        return 0;
    }

    m_metadata=new char[m_metadataLen];
    err=recv(m_connFd,m_metadata,m_metadataLen,0);
    return err;    
}
int Protocol::Packet::readPayload()
{
    /*
    * set m_payload and m_payloadLen variable if
    *   -The 8 Bytes of the m_connFd can be parsed to a int (say  X)
    *   -and reading on m_connFd yields len specified by X
    * 
    * Return:
    *   -1 on failure, lenght of payload read othewise
    */

    char buffer[9];

    int err=recv(m_connFd,buffer,8,0);
    if(err<8)
        return -1;

    buffer[8]='\0';
    m_payloadLen=atoi(buffer);

    if(m_payloadLen==0)
    {
        for(int i=0;i<8;i++)
            if(buffer[i]!='0')
                return -1;

        return 0;
    }
    m_payload=new char[m_payloadLen];
    err=recv(m_connFd,m_payload,m_payloadLen,0);
    return err; 
}
int Protocol::Packet::readPacket()
{
    int err=readPacketType();
    if(err==-1)
        return -1;

    err=readMetadata();
    if(err==-1)
        return -1;

    err=readPayload();
    if(err==-1)
        return -1;

    return 0;
}
const char* Protocol::Packet::getMetadata() const { return m_metadata; }
const char* Protocol::Packet::getPayload() const { return m_payload; }
int Protocol::Packet::getMetadataLen() const { return m_metadataLen; }
int Protocol::Packet::getPayloadLen() const { return m_payloadLen; }
int Protocol::Packet::getPacketType() const { return m_packetType; }
//-----------------------------------------



