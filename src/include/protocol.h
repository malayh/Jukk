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
            virtual int readMetaData();
            virtual int process()=0;
            virtual ~Command();
            Command(int);
    };    

    class Heartbeat:public Command
    {
        private:
        Heartbeat();
        int processPayload();

        public:
        int process() override;
        Heartbeat(int);
        ~Heartbeat();
    };

    std::string readCommandFromNewConnection(int);
    Command* getCommandHandler(std::string,int);


}

#endif