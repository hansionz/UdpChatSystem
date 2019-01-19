#pragma once 

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include  <map>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <data_pool.h>

#define SIZE 1024 //发送信息的最大长度

class UdpServer
{
public:
  UdpServer(uint16_t _port);

  void InitServer();

  //服务器收数据
  void RecvData(std::string &out_string);

  //服务器向目的地址单向发送消息
  void SendData(const std::string &in_string, const struct sockaddr_in &peer);

  //服务器向除发送发以外的所有人发送消息
  void BroadCast();

  ~UdpServer();

private:
  int sock;
  int port;
  DataPool pool;
  std::map<uint32_t, struct sockaddr_in> online;
};
