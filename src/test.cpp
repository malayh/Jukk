#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

class Base
{
    public:
    virtual ~Base()
    {
        std::cout<<"Base destuctor."<<std::endl;
    }
};

class Derived: public Base
{
    public:
    virtual ~Derived()
    {
        std::cout<<"Derived dest."<<std::endl;
    }
};
int main(int argc, char const *argv[])
{
    Base *bPtr = new Derived();
    delete bPtr;  
    return 0;
}
