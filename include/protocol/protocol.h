#ifndef protocol_h_
#define protocol_h_
#include<iostream>
#include<vector>
#include<utility>

#include "util.h"

namespace Protocol
{
    typedef enum{
        HEART_BEAT
    } PacketType;

    class Packet
    {
        private:
            char *m_metadata;
            char *m_payload;
            int m_packetType,m_connFd,m_metadataLen,m_payloadLen;
            Util::Logger *m_logger;
            Packet();
            int readPacketType();
            int readMetadata();
            int readPayload();
        public:
            Packet(int,Util::Logger*);
            ~Packet();
            int readPacket();
            const char* getMetadata() const;
            const char* getPayload() const;
            int getPayloadLen() const;
            int getMetadataLen() const;
            int getPacketType() const;
            int getConnFd() const;
    };
    
    class PacketBuffer
    {
        private:
            int m_packetType, m_metaLen, m_payloadLen;
            std::vector<std::pair<int,char*>> m_metadata;
            std::vector<std::pair<int,char*>> m_payload;
        public:
            PacketBuffer();
            PacketBuffer(const PacketBuffer&) = delete;
            ~PacketBuffer();

            int setPacketType(int);

            int putMetadata(const char*,int);
            int putMetadata(const std::string&);

            int putPayload(const char *,int);
            int putPayload(const std::string&);

            int clearMetadata();
            int clearPayload();

            int sendPacketOnFd(int);
    };
}

#endif