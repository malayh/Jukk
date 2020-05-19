#include<iostream>
#include<cstdlib>
#include<math.h>
#include<cstring>


int lpadIntToStr(int num,char *buffer, int len)
{
    /*
    * It will lpad 'num' with 0s till the desired value of 'len'
    * 'buffer' has have enough space to accomodate 'len+1' number of characters, because of \0 at the end 
    */

    if(num>pow(10,len)-1 || num<0)
        return -1;
        
    //this is clever! I like it!
    buffer[len]='\0';
    for(;--len>=0;num/=10)
        buffer[len]='0'+(num%10);

    return 0;
}
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
