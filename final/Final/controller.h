#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

typedef struct {
  pthread_cond_t * backrobot_cond;
  pthread_cond_t * frontrobot_cond;
} cv_data
