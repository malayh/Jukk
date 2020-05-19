#ifndef protocol_h_
#define protocol_h_

namespace Protocol
{
    class Command
    {
        private:
            Command(){};
        protected:
            int connFd;
        public:
            virtual ~Command(){};
            virtual int process()=0;
            Command(int fd)
            {
                this->connFd=fd;
            }

    };
    class Heartbeat:public Protocol::Command
    {
        public:
        int process(int);
    };
}

#endif