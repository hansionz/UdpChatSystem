#include "udp_server.h"
#include "data.h"

  //在构造函数中不要做有风险的事
  UdpServer::UdpServer(uint16_t _port)
    :port(_port)
  {}

  void UdpServer::InitServer()
  {
    //创建套接字
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
      std::cerr << "socket error!" << std::endl;
      return;
    }
    //绑定端口号
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //INADDR_ANY代表所有的ip地址
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
      std::cerr << "bind error!" << std::endl;
      return;
      //UDP不需要发送缓冲区,不需要超时重传
      //TCP需要发送缓冲区，需要超时重传(用户不管，所以要存在发送缓冲区)
      //TCP存在接收、发送缓冲区.什么时候发由TCP控制(流量控制、超时重传、拥塞控制)
      //UDP存在接收缓冲区、但不存在发送缓冲区
      //面试官问:UDP丢包怎么办？
      //将TCP中的一些合适的可靠机制放在用户层去实现
    }
  }
  
  //服务器收数据
  void UdpServer::RecvData(std::string &out_string)
  {
    char buf[SIZE];
    //要维护在线用户列表,所以有必要保存对端信息
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    ssize_t rs = recvfrom(sock, buf, SIZE - 1, 0, (struct sockaddr*)&peer, &len);
    if(rs > 0){
      buf[rs] = 0;
      out_string = buf;
      //将服务器收到的消息放到数据池中
      pool.PutMess(out_string);

      //将收到数据反序列化
      Data d;
      d.Unserialize(out_string);
      //Data中的type字段控制客户端退出
      if(d.type == "quit")
      {
        std::map<uint32_t, struct sockaddr_in>::iterator it = online.find(peer.sin_addr.s_addr);
        if(it != online.end())
        {
          online.erase(it->first);//key值
        }
      }
      else
      {
        //将上线的信息维护到在线列表
        online.insert(std::pair<uint32_t, struct sockaddr_in>(peer.sin_addr.s_addr, peer));
      }
    }
  }

//服务器向目的地址发送消息
void UdpServer::SendData(const std::string &in_string, const struct sockaddr_in &peer)
{
    sendto(sock, in_string.c_str(), in_string.size(), 0, (struct sockaddr*)&peer,sizeof(peer));
}

//服务器向在线列表中的所有人发送消息
void UdpServer::BroadCast()
{
  std::string mess;
  //从数据池中取到数据
  pool.GetMess(mess);

  std::map<uint32_t, struct sockaddr_in>::iterator it = online.begin();
  for( ; it != online.end() ; it++ )
  {
    SendData(mess, it->second);//value-->sockaddr
  }
}

UdpServer::~UdpServer()
  {
    close(sock);
    port = -1;
  }
