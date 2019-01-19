#include "data_pool.h"

DataPool::DataPool(int _cap)
    :pool(_cap)
    ,cap(_cap)
  {
    product_step = 0;
    consume_step = 0;
    sem_init(&blank_sem, 0, _cap);
    sem_init(&data_sem, 0, 0);
  }

  void DataPool::GetMess(std::string &out_mess)
  {
    sem_wait(&data_sem);
    out_mess = pool[consume_step];
    consume_step++;
    consume_step %= cap;
    sem_post(&blank_sem);
  }

  void DataPool::PutMess(const std::string &in_mess)
  {
    sem_wait(&blank_sem);
    pool[product_step] = in_mess;
    product_step++;
    product_step %= cap;
    sem_post(&data_sem);
  }

DataPool::~DataPool()
  {
    sem_destroy(&blank_sem);
    sem_destroy(&data_sem);
  }

