#include "data.h"

Data::Data()
{}

//序列化
void Data::Serialize(std::string &out_string)
{
  ValueObj root;
  root["nick_name"] = nick_name;
  root["school"] = school;
  root["message"] = message;
  root["type"] = type;

  Write w;
  w.run(root, out_string);
}

//反序列化
void Data::Unserialize(std::string &in_string)
{
  ValueObj root;
  Read r;
  r.run(in_string, root);

  nick_name = root["nick_name"].getstring();
  school = root["school"].getstring();
  message = root["message"].getstring();
  type = root["type"].getstring();
}

Data::~Data()
{}

//int main()
//{
//  Data d;
//  d.nick_name = "控心";
//  d.school = "XSUT";
//  d.message = "你们好";
//  d.type = "None";
//
//  std::string out_string;
//  d.Serialize(out_string);
//
//  std::cout << out_string << std::endl;
//
//  Data d1;
//  d1.Unserialize(out_string);
//  std::cout << d1.nick_name << std::endl;
//  std::cout << d1.school << std::endl;
//
//  return 0;
//}
