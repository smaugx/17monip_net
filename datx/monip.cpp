#include"monip.h"


bool MONIP::init(const std::string ipdb)
{
  if (this->offset)
  {
    return true;
  }

  FILE *file = fopen(ipdb.c_str(), "rb");
  if(NULL == file)
  {
    return false;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  this->data = (byte *) malloc(size * sizeof(byte));
  fread(this->data, sizeof(byte), (size_t) size, file);

  fclose(file);

  uint indexLength = B2IU(this->data);

  this->index = (byte *) malloc(indexLength * sizeof(byte));
  memcpy(this->index, this->data + 4, indexLength);

  this->offset = indexLength;

  this->flag = (uint *) malloc(65536 * sizeof(uint));
  memcpy(this->flag, this->index, 65536 * sizeof(uint));
  return true;
}

void MONIP::find(const char *ip, char *result)
{
  if (!this->offset)
  {
    //not init
    return ;
  }
  uint ips[4];
  int num = sscanf(ip, "%d.%d.%d.%d", &ips[0], &ips[1], &ips[2], &ips[3]);
  if (num == 4)
  {
    uint ip_prefix_value = ips[0] * 256 + ips[1];
    uint ip2long_value = B2IU(ips);
    uint start = this->flag[ip_prefix_value];
    uint max_comp_len = this->offset - 262144 - 4;
    uint index_offset = 0;
    uint index_length = 0;
    for (start = start * 9 + 262144; start < max_comp_len; start += 9)
    {
      if (B2IU(this->index + start) >= ip2long_value)
      {
        index_offset = B2IL(this->index + start + 4) & 0x00FFFFFF;
        index_length = (this->index[start+7] << 8) + this->index[start+8];
        break;
      }
    }
    memcpy(result, this->data + this->offset + index_offset - 262144, index_length);
    result[index_length] = '\0';
  }
  return ;
}


char * MONIP::strtok_r_2(char *str, char const *delims, char **context)
{
  char *p, *ret = NULL;

  if (str != NULL)
    *context = str;

  if (*context == NULL)
    return NULL;

  if ((p = strpbrk(*context, delims)) != NULL)
  {
    *p = 0;
    ret = *context;
    *context = ++p;
  }
  else if (**context)
  {
    ret = *context;
    *context = NULL;
  }
  return ret;
}

void MONIP::query(std::string ip,std::vector<std::string> &provinceOperator)
{
  char result[512];
  bzero(result,sizeof(result));
  this->find(ip.c_str(),result);
  if(strlen(result) == 0)
  {
    return;
  }
  char *rst = NULL;
  char *lasts;
  rst = strtok_r_2(result, "\t", &lasts);
  while (rst)
  {
    //printf("%s\n", rst);
    provinceOperator.push_back(rst);
    std::cout<<"rst: "<<rst<<std::endl;
    rst = strtok_r_2(NULL, "\t", &lasts);
  }
}
