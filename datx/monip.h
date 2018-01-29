#ifndef _H_MONIP_
#define _H_MONIP_

#include<iostream>
#include<string>
#include<vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef unsigned char byte;
typedef unsigned int uint;
#define B2IL(b) (((b)[0] & 0xFF) | (((b)[1] << 8) & 0xFF00) | (((b)[2] << 16) & 0xFF0000) | (((b)[3] << 24) & 0xFF000000))
#define B2IU(b) (((b)[3] & 0xFF) | (((b)[2] << 8) & 0xFF00) | (((b)[1] << 16) & 0xFF0000) | (((b)[0] << 24) & 0xFF000000))


class MONIP;

class MONIP
{
public:
  MONIP()
  {
    this->data = NULL;
    this->index = NULL;
    this->flag = NULL;
    this->offset = 0;
  }

  MONIP(MONIP const&) = delete;
  MONIP& operator=(MONIP const&) = delete;

  bool init(const std::string ipdb);
  void find(const char *ip, char *result);

  char *strtok_r_2(char *str, char const *delims, char **context);

  void query(std::string ip,std::vector<std::string> &provinceOperator);

private:
  byte * data;
  byte * index;
  uint * flag;
  uint offset;

public:
  ~MONIP()
  {
    if (!this->offset)
    {
      return ;
    }
    free(this->flag);
    free(this->index);
    free(this->data);
    this->offset = 0;
    return ;
  }
};

#endif
