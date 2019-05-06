#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <pthread.h>
#include "controller.h"

typedef struct t_robot {

  int ix;
  pthread_mutex_t robot_lock;

  pthread_cond_t * robot_cond;
  
  cv_data data;
  void (*run) (struct t_robot * robot);
} robot;

robot * create_robot(int ix, pthread_cond_t * cond);
//robot * create_robot(int ix, cv_data * data);
void run(struct t_robot * robot);


#endif
