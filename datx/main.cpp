#include<iostream>
#include<vector>
#include<string>
#include"monip.h"
#include<memory>

using namespace std;


int main(int argc,char* argv[])
{
  if(argc != 2)
  {
    cout<<"error: usage: ./main 127.0.0.1"<<endl;
    return 0;
  }
  //MONIP myip("./17monip.datx");
  shared_ptr<MONIP> myip = make_shared<MONIP>();
  //myip->init("./17monip.datx");
  myip->init("./mydata4vipday2.datx");
  vector<string> result;
  myip->query(argv[1],result);
  if(result.empty())
  {
    return 0;
  }
  cout<<"country: "<<result[0]<<" province: "<<result[1]<<" operator:"<<result[4]<<endl;
  /*
  cout<<"length = "<<result.size()<<endl;
  for(vector<string>::iterator i = result.begin(); i != result.end(); i++)
  {
    cout<<"hello: "<<*i<<endl;
  }
  */
  return 0;
}
