#pragma once 

#include <iostream>
#include <string>
#include <value.h>
#include <write.h>
#include <read.h>

// nick_name("zs") , school("XUST"), message("hello"), type("none") 
// json串:"{nick_name:"zs",school:"XUST",message:"hello",type:"None"}" 

class Data
{
public:
  Data();
  //序列化
  void Serialize(std::string &out_string);
  //反序列化
  void Unserialize(std::string &in_string);
  ~Data();

public:
  std::string nick_name;
  std::string school;
  std::string message;
  std::string type;      //保留字段,用来做退出处理
};
