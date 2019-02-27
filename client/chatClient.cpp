#include "udp_client.h"
#include "window.h"
#include "data.h"
#include <signal.h>
#include <pthread.h>
#include <vector>

volatile int is_quit = 0;//退出标志位

typedef struct{
  UdpClient *clientp;
  Window *winp;//窗口程序
  std::string nick_name;
  std::string school;
}client_info_t;

client_info_t cw; //客户端信息 nick_name-scholl:message

std::vector<std::string> online; //客户端展示的在线好友列表

//将新上线的好友增加到客户端界面上
static void AddUser(std::string &f)
{
  //在线好友列表不能重复.先查找如果没有在插入
  std::vector<std::string>::iterator it = online.begin();
  for(; it != online.end() ; it++)
  {
    if(*it == f){
      return;
    }
  }
  online.push_back(f);
}

//如果某个客户端退出,从客户端的在线好友界面删除掉该好友
static void DelUser(std::string &f)
{
  //先查找如果存在则删除
  std::vector<std::string>::iterator it = online.begin();
  for(; it != online.end() ; it++)
  {
    //删除之后必须break,否则迭代器失效
    if(*it == f){
      online.erase(it);
      break;
    }
  }
}

//标题窗口绘制线程
void *RunHeader(void* arg)
{
  client_info_t *cwp = (client_info_t*)arg;
  Window *wp = cwp->winp;
  wp->DrawHeader();
  std::string title = "Welcome To My ChatRoom";

  //绘制标题动态效果
  size_t i = 1;
  int y,x;    //y:行 x:列
  int dir = 0;//方向
  while(1)
  {
    //行不变,列在变,重新刷新,可以完成左右移动功能
    wp->DrawHeader();
    getmaxyx(wp->GetHeader(), y, x);//获得该窗口的行和列
    wp->PutWindow(wp->GetHeader(), y/2, i, title);
    if(i >= x - title.size() - 3)
    {
      dir = 1;//right->left
    }
    if(i <= 4)
    {
      dir = 0;//left->right
    }
    if(dir == 0)
      i++;
    else 
      i--;
    usleep(100000);
  }
}

// 输入窗口绘制线程
void *RunInput(void* arg)
{
  client_info_t *cwp = (client_info_t*)arg;
  Window *wp = cwp->winp;
  UdpClient *cp = cwp->clientp;

  wp->DrawInput();
  std::string message = "Please Enter$ ";
  std::string str;
  Data d;
  std::string out_string;
  while(1)
  {
    wp->PutWindow(wp->GetInput(), 1, 2, message);
    //从窗口获得数据，发送到服务器
    wp->GetWindow(wp->GetInput(), str);

    //序列化
    d.nick_name = cwp->nick_name;
    d.school = cwp->school;
    d.type = "None";
    d.message = str;
    d.Serialize(out_string);

    cp->SendData(out_string);//->server

    wp->DrawInput();
  }
}

//输出和在线列表绘制线程
void *RunOutputOnline(void* arg)
{
  client_info_t *cwp = (client_info_t*)arg;
  Window *wp = cwp->winp;
  UdpClient *cp = cwp->clientp;

  wp->DrawOutput();
  wp->DrawOnline();

  int y,x;
  int i = 1;
  std::string out_string;//json
  std::string show_string;
  Data d;
  while(1){
    cp->RecvData(out_string);
    //将收到的数据反序列化打印到Output上
    d.Unserialize(out_string);
    //zs-xust message 
    show_string = d.nick_name;
    show_string += "-";
    show_string += d.school;

    if(d.type == "quit")
    {
      DelUser(show_string);
    }
    else
    {
      AddUser(show_string);
      show_string += ": ";
      show_string += d.message;

      //避免超出矩形框
      if(i > y -2)
      {
        i = 1;
        wp->DrawOutput();
      }
      getmaxyx(wp->GetOutput(),y,x);
      //y change x no change
      
      wp->PutWindow(wp->GetOutput(), i++, 2, show_string);
    }
    //online
    wp->DrawOnline();
    size_t j = 0;
    for(; j < online.size(); j++)
    {
      wp->PutWindow(wp->GetOnline(), j+1, 2, online[j]);
    }
  }
}

void SendQuit(int sig)
{
  Data d;
  d.nick_name = cw.nick_name;
  d.school = cw.school;
  d.message = "None";
  d.type = "quit";

  std::string out_string;
  d.Serialize(out_string);

  cw.clientp->SendData(out_string);
  is_quit = 1;
}

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cout << "Usage:" << argv[1] << " [server_ip][server_port]" << std::endl; 
    return 1;
  }

  std::cout << "Please Enter You NickName$ ";//昵称
  std::cin >> cw.nick_name;
  std::cout << "Please Enter You School$ ";  //学校
  std::cin >> cw.school;

  //注册SIGINT的处理函数
  signal(SIGINT, SendQuit);

  UdpClient client(argv[1], atoi(argv[2]));
  client.InitClient();

  Window w;
  cw.clientp = &client;
  cw.winp = &w;

  pthread_t header,output_online,input;
  pthread_create(&header, NULL, RunHeader, (void *)&cw);
  pthread_create(&output_online, NULL, RunOutputOnline, (void *)&cw);
  pthread_create(&input, NULL, RunInput, (void*)&cw);
  
  //主线程每隔1秒检测一次
  while(!is_quit){
    sleep(1);
  }

  //取消线程
  pthread_cancel(header);
  pthread_cancel(output_online);
  pthread_cancel(input);
  //pthread_join(header, NULL);
  //pthread_join(output_online, NULL);
  //pthread_join(input, NULL);
  // std::string message;
  // while(1)
  // {
  //   std::cout << "Please Enter$ ";
  //   std::cin >> message;
  //   client.SendData(message);
  // }
  return 0;
}
