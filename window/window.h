#pragma once 

#include <iostream>
#include <ncurses.h>

#define SIZE 1024

class Window
{
public:
  Window();
  //绘制4个窗口
  void DrawHeader();
  void DrawOutput();
  void DrawInput();
  void DrawOnline();
  //向该窗口写入字符串(y:高度,纵坐标 x:宽度,横坐标)
  void PutWindow(WINDOW *w, int y, int x,std::string& message);  
  //从该窗口获得字符串
  void GetWindow(WINDOW *w, std::string &out_string);

  //获得4个窗口的句柄
  WINDOW* GetHeader();
  WINDOW* GetOutput();
  WINDOW* GetInput();
  WINDOW* GetOnline();
  ~Window();
private:
  WINDOW *header;
  WINDOW *output;
  WINDOW *input;
  WINDOW *online;
};
