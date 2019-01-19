#include "udp_client.h"

UdpClient::UdpClient(std::string server_ip, uint16_t server_port)
{
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);
  server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
}

void UdpClient::InitClient()
{
  //创建套接字
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0){
    std::cerr << "socket error!" << std::endl;
    return;
  }
}
  
//客户端收数据
void UdpClient::RecvData(std::string &out_string)
{
  char buf[SIZE];
  struct sockaddr_in peer;
  socklen_t len = sizeof(peer);
  ssize_t rs = recvfrom(sock, buf, SIZE - 1, 0, (struct sockaddr*)&peer, &len);
  if(rs > 0){
    buf[rs] = 0;
    out_string = buf;
  }
}

//客户端发送数据
void UdpClient::SendData(const std::string &in_string)
{
  sendto(sock, in_string.c_str(), in_string.size(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
}

UdpClient::~UdpClient()
{
  close(sock);
}

