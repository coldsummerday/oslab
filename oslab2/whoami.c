#define __LIBRARY__                    
#include <unistd.h>                
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

_syscall2(int, whoami,char*,name,unsigned int,size);

int main(int argc ,char *argv[])
{
    char *username;
    int counter;  

    username = (char *)malloc(sizeof(char)*128);  
    counter = whoami(username,128);  
    printf("%s\n",username); 
    free(username); 
    return 0;
}