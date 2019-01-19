#pragma once 

#include <iostream>
#include <vector>
#include <semaphore.h>

#define NUM 256

class DataPool
{
public:
  DataPool(int _cap = NUM);

  void GetMess(std::string &out_mess);

  void PutMess(const std::string &in_mess);

  ~DataPool();

private:
  std::vector<std::string> pool;
  int cap;//环形队列的长度
  int consume_step;
  int product_step;
  sem_t blank_sem;
  sem_t data_sem;
};
