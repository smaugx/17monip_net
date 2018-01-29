#include <stdio.h>
#include "ipip.h"

int main(int argc,char* argv[]) {
    if(argc != 2)
    {
      printf("error: usage: ./main 127.0.0.1 \n");
      return -1;
    }
    init("17monipdb.dat");
    //char *ip = "8.8.8.8";
    char *ip = argv[1];
    char result[128];
    find(ip, result);
    printf("%s -> %s\n", ip, result);
    destroy();
    return 0;
}
