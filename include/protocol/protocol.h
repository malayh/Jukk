#ifndef protocol_h_
#define protocol_h_
#include<iostream>

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
            Packet();
            int readPacketType();
            int readMetadata();
            int readPayload();
        public:
            Packet(int);
            ~Packet();
            int readPacket();
            const char* getMetadata() const;
            const char* getPayload() const;
            int getPayloadLen() const;
            int getMetadataLen() const;
            int getPacketType() const;
            int getConnFd() const;
            

    };
}

#endif