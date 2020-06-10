#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

#include "protocol/protocol.h"


/*
*   Packet
*   :   TCPServer reads packet.
*/


Protocol::Packet::Packet(){}
Protocol::Packet::~Packet()
{
    if(m_metadataLen>0)
        delete[] m_metadata;
    if(m_payloadLen>0)
        delete[] m_payload;

    close(m_connFd);
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
    *   
    *   Notice that m_metadata is NOT null terminated
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

    //Notice that metadata is not null terminated.
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
    *   
    *   Notice that m_payload is not null terminated
    * 
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
    //Notice that payload is not null terminated
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
        return -2;

    err=readPayload();
    if(err==-1)
        return -3;

    return 0;
}
const char* Protocol::Packet::getMetadata() const { return m_metadata; }
const char* Protocol::Packet::getPayload() const { return m_payload; }
int Protocol::Packet::getMetadataLen() const { return m_metadataLen; }
int Protocol::Packet::getPayloadLen() const { return m_payloadLen; }
int Protocol::Packet::getPacketType() const { return m_packetType; }
int Protocol::Packet::getConnFd() const { return m_connFd; }
//-----------------------------------------



