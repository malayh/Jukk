/*
*   Testing Protocol::*
*/

#include<assert.h>
#include "protocol/protocol.h"
#include "util.h"

void test_Packet()
{
    /*
        This will only work if __TESTING__ is defined in protocol.cpp
    */

    // For valid packet
    FILE *fp=fopen("../resource/test/packet_valid.txt","r");
    int fd=fileno(fp);

    char *m="metadata";
    char *p="payload";

    Util::Logger logger("../resource/test/packetTest.log",Util::Logger::TRACE);
    Protocol::Packet pkt(fd,&logger);
    int err=pkt.readPacket();
    assert(err==0);

    assert(pkt.getPacketType()==0);
    assert(pkt.getMetadataLen()==8);
    assert(pkt.getPayloadLen()==7);
    
    const char *meta=pkt.getMetadata();
    const char *payload=pkt.getPayload();

    for(int i=0;i<pkt.getMetadataLen();i++)
        assert(meta[i]==m[i]);

    for(int i=0;i<pkt.getPayloadLen();i++)
        assert(payload[i]==p[i]);

    fclose(fp);
}

void test_PacketBuffer()
{
    Protocol::PacketBuffer pb;
    std::string m_1="M_Lenght:11asdf78723409";
    std::string p_1="P_Lenght:11";

    pb.putMetadata(m_1);
    pb.putPayload(p_1);

    std::string m_2="M2_Lenght:12";
    std::string p_2="P2_Lenght:12sfdajklsfjkd";
    pb.putMetadata(m_2.c_str(),m_2.length());
    pb.putPayload(p_2.c_str(),p_2.length());

    pb.setPacketType(1);

    FILE *fp=fopen("../resource/test/packe_buffer.txt","w");
    int fd=fileno(fp);

    pb.sendPacketOnFd(fd);

    std::cout<<"Meta: "<<m_1.length()+m_2.length()<<std::endl;
    std::cout<<"Payload: "<<p_1.length()+p_2.length()<<std::endl;


    fclose(fp);

}
