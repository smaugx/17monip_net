#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
 
class IP17MON {
public:
    bool init(const std::string strPathToDataFile = "./17monipdb.dat") {
        printf("Opening %s\n", strPathToDataFile.c_str());
        std::ifstream ifDataFile(strPathToDataFile.c_str(), std::ios::binary);
        if (ifDataFile.is_open() == false) {
            printf("%m\n");
            return false;
        }
        // std vector reserve
        vecDataFile.assign(std::istreambuf_iterator<char>(ifDataFile), std::istreambuf_iterator<char>());
        printf("Load %lu bytes success\n", vecDataFile.size());
         
        unsigned int uiIndexLen = 0;
        memcpy(&uiIndexLen, &vecDataFile[0], 4);
        uiIndexLen = ntohl(uiIndexLen);
        pIPIndex = &vecDataFile[4];
        pIPData = &vecDataFile[uiIndexLen];
         
        return true;
    }
    std::string find(const std::string strIP) {        
        struct sockaddr_in stSockAddrInet = {0};
        if(inet_aton(strIP.c_str(), &stSockAddrInet.sin_addr) == 0) {
            //printf("convert error\n");
            return "";
        }
        unsigned int uiIP = ntohl(stSockAddrInet.sin_addr.s_addr);
        
        // atoi The string can contain additional characters after those 
        // that form the integral number, which are ignored and have no 
        // effect on the behavior of this function.
        int iFirst = atoi(strIP.c_str());
        int iStart = 0;
        memcpy(&iStart, pIPIndex+(iFirst*4), 4);
        int iMaxComLen = pIPData - pIPIndex - 1024 - 4;
         
        int iIndexOffset = -1;
        unsigned char ucIndexLength = 0;
        for (iStart = iStart * 8 + 1024; iStart < iMaxComLen; iStart += 8) {
            unsigned int uiCurrIP = 0;
            memcpy(&uiCurrIP, pIPIndex+iStart, 4);
            uiCurrIP = ntohl(uiCurrIP);
            if (uiCurrIP >= uiIP) {
                iIndexOffset = 0;
                memcpy(&iIndexOffset, pIPIndex+iStart+4, 3);   
                memcpy(&ucIndexLength, pIPIndex+iStart+7, 1);  
                break;
            }
        }
         
        if (iIndexOffset == -1) {
            return "";
        }
         
        std::string strRegion(pIPData + iIndexOffset - 1024, ucIndexLength);
        return strRegion;
    }
private:
    std::vector<char> vecDataFile;
    char *pIPIndex, *pIPData;
};
 
int main(int argc,char* argv[]) {
    if(argc != 2)
    {
      printf("error: usage: ./test 127.0.0.1\n");
      return -1;
    }
    IP17MON oIP17MON;
    assert(oIP17MON.init());
    char* ip = argv[1];
    printf("%s -> %s\n", ip, oIP17MON.find(ip).c_str());    
    return 0;
}
