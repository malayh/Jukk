#ifndef protocol_h_
#define protocol_h_
#include<iostream>

namespace Protocol
{
    typedef enum{
        HEART_BEAT
    } PacketType;

    class Command
    {
        protected:
            Command();
            int connFd;
            std::string metaData;

        public:
            virtual void setConnFd(int);
            virtual ~Command();
            virtual int readMetaData();
            virtual bool isStateLess()=0;
            virtual int process()=0;
            Command(int);
    };    

    class Heartbeat:public Command
    {
        private:
        Heartbeat();
        int processPayload();

        public:
        int process() override;
        bool isStateLess() override;
        Heartbeat(int);
        ~Heartbeat();
    };

    std::string readCommandFromNewConnection(int);
    Command* getCommandHandler(std::string,int);

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
            

    };
}

#endif