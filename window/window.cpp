#include "window.h"
#include <unistd.h>

Window::Window()
{
  initscr();//COLS, LINES
}
//获取四个窗口的句柄
WINDOW* Window::GetHeader()
{
  return header;
}
WINDOW* Window::GetOutput()
{
  return output;
}
WINDOW* Window::GetInput()
{
  return input;
}
WINDOW* Window::GetOnline()
{
  return online;
}

//向矩形框中放入数据
void Window::PutWindow(WINDOW *w, int y, int x,std::string& message)
{
  //将光标移至矩形框并向该矩形框写入字符串
  mvwaddstr(w, y, x, message.c_str());
  wrefresh(w);//刷新边框
}
//从矩形框中获得数据
void Window::GetWindow(WINDOW *w, std::string &out_string)
{
  char buf[SIZE];
  wgetnstr(w, buf, SIZE);
  out_string = buf;
}

void Window::DrawHeader()
{
  int y = 0;
  int x = 0;
  int h = LINES / 5;
  int w = COLS;

  header = newwin(h, w, y, x);
  box(header, '+', '=');//绘制边框+代表列=代表行
  wrefresh(header);//刷新边框
}

void Window::DrawInput()
{
  int y = LINES*4 / 5;
  int x = 0;
  int h = LINES / 5;
  int w = COLS;

  input = newwin(h, w, y, x);
  box(input, '-', '-');//绘制边框+代表列=代表行
  wrefresh(input);//刷新边框
}

void Window::DrawOutput()
{
  int y = LINES / 5;
  int x = 0;
  int h = LINES*3 / 5;
  int w = COLS*3 /4;

  output = newwin(h, w, y, x);
  box(output, '-', '-');
  wrefresh(output);
}

void Window::DrawOnline()
{
  int y = LINES / 5;
  int x = COLS*3 / 4;
  int h = LINES*3 / 5;
  int w = COLS/4;

  online = newwin(h, w, y, x);
  box(online, '-', '-');
  wrefresh(online);
}

Window::~Window()
{
  //结束初始化
  endwin();
}

//int main()
//{
  //Window w;
  //w.DrawHeader();
  //w.DrawOutput();
  //w.DrawOnline();
  //w.DrawInput();
  //std::string title = "Welcome To My ChatRoom";
  //size_t i = 1;
  //int y,x;
  //int dir = 0;//方向left->right right->left
  //while(1)
  //{
  //  //行不变，列在变，重新刷新，可以完成移动功能
  //  w.DrawHeader();
  //  getmaxyx(w.GetHeader(), y, x);//获得窗口的高和宽

  //  //居中
  //  w.PutWindow(w.GetHeader(), y/2, i, title);

  //  //方向left->right right->left
  //  if(i >= x - title.size() - 3)
  //  {
  //    dir = 1;//left<-right
  //  }
  //  if( i <= 4)
  //  {
  //    dir = 0;//left->right
  //  }
  //  if(dir == 0)
  //    i++;
  //  else 
  //    i--;
  //  usleep(100000);
  //}
  //sleep(3);
  //return 0;
//}
