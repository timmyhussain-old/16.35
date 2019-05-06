#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <pthread.h>

typedef struct t_robot {

  int ix;
  pthread_mutex_t robot_lock;

  void * run (struct * t_robot);
} robot;


robot * create_robot (int ix, struct cv_data)
void run(struct * t_robot);
