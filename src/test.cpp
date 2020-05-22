#include<iostream>
#include<cstdlib>
#include<math.h>
#include<cstring>

#include "protocol.h"
#include "util.h"

int main(int argc, char const *argv[])
{

    char strNum[100];
    int n=5678;
    int x=lpadIntToStr(n,strNum,45);
    std::cout<<strNum<<std::endl;
    // std::cout<<strNum[3]<<std::endl;
    std::cout<<strlen(strNum)<<std::endl;
    std::cout<<atoi(strNum)<<std::endl;
    return 0;
}
