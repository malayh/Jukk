#include<iostream>
#include<unistd.h>
#include<string.h>

#include "protocol/protocol.h"
#include "util.h"

/* 
*   When __TESTING__ is defined:
*   : To be able to use a file a socket. Idk if this the right way to do it or not
*
*   : recv will be replaced by read
*   : send will be replaced by write*
*/
// #define __TESTING__

#ifndef __TESTING__

    #include<sys/socket.h>

#else

    int recv(int fd,void* buffer,int size,int flag)
    {
        return read(fd,buffer,size);
    }
    int send(int fd,void *buffer,int size,int flag)
    {
        return write(fd,buffer,size);
    }

#endif


//-------- Protocol::Packet ---------------------------------

/*
*   Packet
*   :   TCPServer reads packet.
*/

Protocol::Packet::Packet(){}
Protocol::Packet::~Packet()
{
    //Remember this one closed the fd

    if(m_metadataLen>0)
        delete[] m_metadata;
    if(m_payloadLen>0)
        delete[] m_payload;
    
    close(m_connFd);
}
Protocol::Packet::Packet(int fd,Util::Logger *logger)
{
    m_connFd=fd;
    m_packetType=-1;
    m_payloadLen=-1;
    m_metadataLen=-1;
    m_logger=logger;
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
    m_logger->trace("Packet::readPacketType","Reading packet type on FD:"+std::to_string(m_connFd));

    char buffer[9];
    int err=recv(m_connFd,buffer,8,0);

    if( err<8 ) 
        return -1;
    
    buffer[8]='\0';
    int packet=atoi(buffer);

    m_logger->trace("Packet::readPacketType","FD: "+std::to_string(m_connFd)+" Packet type buffer:"+std::string(buffer));
    if(packet==0)
    {
        for(int i=0;i<8;i++)
            if(buffer[i]!='0')
                return -1;
    }
    m_logger->trace("Packet::readPacketType","FD: "+std::to_string(m_connFd)+"Packet Type:"+std::to_string(packet));
    
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
    *   
    *   Notice that m_metadata is NOT null terminated
    */
    m_logger->trace("Packet::readMetadata","Reading metadata on FD:"+std::to_string(m_connFd));

    char buffer[9];

    int err=recv(m_connFd,buffer,8,0);
    if(err<8)
        return -1;

    buffer[8]='\0';
    m_logger->trace("Packet::readMetada","FD: "+std::to_string(m_connFd)+" Metalen buffer:"+std::string(buffer));
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

    
    //Notice that metadata is not null terminated.
    m_metadata=new char[m_metadataLen];
    err=recv(m_connFd,m_metadata,m_metadataLen,0);

    m_logger->debug("Packet::readMetada",
                    "FD: "+std::to_string(m_connFd)+
                    " Metalen:"+std::to_string(m_metadataLen)+
                    " Metadata: "+std::string(m_metadata,m_metadataLen));
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
    *   
    *   Notice that m_payload is not null terminated
    * 
    */

    m_logger->trace("Packet::readPayload","Reading payload on FD:"+std::to_string(m_connFd));

    char buffer[9];

    int err=recv(m_connFd,buffer,8,0);
    if(err<8)
        return -1;

    buffer[8]='\0';
    m_payloadLen=atoi(buffer);
    m_logger->trace("Packet::readPayload","FD: "+std::to_string(m_connFd)+" PayloadLen buffer:"+std::string(buffer));

    if(m_payloadLen==0)
    {
        for(int i=0;i<8;i++)
            if(buffer[i]!='0')
                return -1;

        return 0;
    }
    //Notice that payload is not null terminated
    m_payload=new char[m_payloadLen];
    err=recv(m_connFd,m_payload,m_payloadLen,0);
    m_logger->debug("Packet::readPayload",
                    "FD: "+std::to_string(m_connFd)+
                    " PayloadLen:"+std::to_string(m_payloadLen)+
                    " Payload: "+std::string(m_payload,m_payloadLen));
    return err; 
}
int Protocol::Packet::readPacket()
{
    m_logger->trace("Packet::readPacket","Reading packet on FD:"+std::to_string(m_connFd));

    int err=readPacketType();
    if(err==-1)
    {
        m_logger->debug("Packet::readPacket","Packet type Incorrect on FD:"+std::to_string(m_connFd));
        return -1;
    }

    err=readMetadata();
    if(err==-1)
    {
        m_logger->debug("Packet::readPacket","Metadata Incorrect on FD:"+std::to_string(m_connFd));
        return -2;
    }

    err=readPayload();
    if(err==-1)
    {
        m_logger->debug("Packet::readPacket","Payload Incorrect on FD:"+std::to_string(m_connFd));
        return -3;
    }

    return 0;
}
const char* Protocol::Packet::getMetadata() const { return m_metadata; }
const char* Protocol::Packet::getPayload() const { return m_payload; }
int Protocol::Packet::getMetadataLen() const { return m_metadataLen; }
int Protocol::Packet::getPayloadLen() const { return m_payloadLen; }
int Protocol::Packet::getPacketType() const { return m_packetType; }
int Protocol::Packet::getConnFd() const { return m_connFd; }


//-------- Protocol::PackerBuffer --------------------------------
/*
*   Protocol::PacketBuffer
*   :   It keeps chunks of meta data *COPIED* in memory. You can write the packet onto a fd when buffer is ready
*       It stores chunks in a vector::pair where pair->first=size of chuch  pair->second=chunk
*/
Protocol::PacketBuffer::PacketBuffer()
{
    m_payloadLen=0;
    m_metaLen=0;
    m_packetType=-1;
}

Protocol::PacketBuffer::~PacketBuffer()
{
    for(auto i: m_metadata)
        if (i.first>0)
            delete[] i.second;
    
    for(auto i:m_payload)
        if(i.first>0)
            delete[] i.second;
}

int Protocol::PacketBuffer::setPacketType(int type)
{
    m_packetType=type;
}

int Protocol::PacketBuffer::putMetadata(const std::string &str)
{

    if(!str.length())
        return 0;

    // Not putting the \0 from str.c_str() in meta
    char *meta = new char[str.length()];
    memcpy(meta,str.c_str(),str.length());
    m_metadata.push_back(std::pair<int,char*>(str.length(),meta));

    m_metaLen+=str.length();

    return str.length();
}

int Protocol::PacketBuffer::putMetadata(const char *str,int len)
{
    if(!len)
        return 0;

    char *meta = new char[len];
    memcpy(meta,str,len);
    m_metadata.push_back(std::pair<int,char*>(len,meta));

    m_metaLen+=len;

    return len;
}

int Protocol::PacketBuffer::putPayload(const std::string &str)
{
    if(!str.length())
        return 0;

    char *pl = new char[str.length()];
    memcpy(pl,str.c_str(),str.length());
    m_payload.push_back(std::pair<int,char*>(str.length(),pl));

    m_payloadLen+=str.length();

    return str.length();
}

int Protocol::PacketBuffer::putPayload(const char *str,int len)
{
    if(!len)
        return 0;

    char *pl = new char[len];
    memcpy(pl,str,len);
    m_payload.push_back(std::pair<int,char*>(len,pl));

    m_payloadLen+=len;

    return len;
}

int Protocol::PacketBuffer::sendPacketOnFd(int fd)
{
    /*
    *   writes buffer to fd. 
    *   Returns:
    *        0 on success
    *       -1 if type is not set
    *       -2 if error occures during sending packet type
    *       -3 if error occures during sending meta data
    *       -4 if error occures during sending payload    *  
    */

    if(m_packetType==-1)
        return -1;

    int err;
    char buffer[9];

    // Sending packet type
    Util::padIntToStr(m_packetType,buffer,8);
    err=send(fd,buffer,8,0);
    if(err==-1)
        return -2;

    // Sending metadata length
    Util::padIntToStr(m_metaLen,buffer,8);
    err=send(fd,buffer,8,0);
    if(err==-1)
        return -3;

    // sending metadata chunks
    for(auto i:m_metadata)
    {
        err=send(fd,i.second,i.first,0);
        if(err==-1)
            return -3;
    }
    
    // sending payload lenght
    Util::padIntToStr(m_payloadLen,buffer,8);
    err=send(fd,buffer,8,0);
    if(err==-1)
        return -4;

    //sending payload chunks
    for(auto i:m_payload)
    {
        err=send(fd,i.second,i.first,0);
        if(err==-1)
            return -4;
    }

    return 0;
    
}