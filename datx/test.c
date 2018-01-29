#include <stdio.h>
#include <string.h>
#include "ipip.h"

char *strtok_r_2(char *str, char const *delims, char **context) {
    char *p, *ret = NULL;

    if (str != NULL)
        *context = str;

    if (*context == NULL)
        return NULL;

    if ((p = strpbrk(*context, delims)) != NULL) {
        *p = 0;
        ret = *context;
        *context = ++p;
    }
    else if (**context) {
        ret = *context;
        *context = NULL;
    }
    return ret;
}

int main(int argc ,char* argv[]) {
    if(argc != 2)
    {
      printf("error: usage: ./main 127.0.0.1 \n");
      return -1;
    }
    init("17monip.datx");
    char *ip = argv[1];
    char result[128];
    bzero(result,sizeof(result));
    find(ip, result);
        printf("%s -> %s\n", ip, result);
        char *rst = NULL;
        char *lasts;
        rst = strtok_r_2(result, "\t", &lasts);
        while (rst) {
            printf("%s\n", rst);
            rst = strtok_r_2(NULL, "\t", &lasts);
        }
    destroy();

    return 0;
}
