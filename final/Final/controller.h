#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <pthread.h>

typedef struct {
  pthread_cond_t * robot_cond;
} cv_data;

cv_data * create_cv_data(pthread_cond_t * cond);

#endif
