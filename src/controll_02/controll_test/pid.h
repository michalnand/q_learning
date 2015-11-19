#ifndef _PID_H_
#define _PID_H_

#include "../common.h"

struct sPID
{
  float kp, ki, kd;
  float k1, k2, k3;
  float e0, e1, e2;
  float res, antiwindup;
};


void pid_init(struct sPID *pid, float kp, float ki, float kd, float antiwindup);
void pid_set(struct sPID *pid, float kp, float ki, float kd, float antiwindup);
void pid_set_dif(struct sPID *pid, float dkp, float dki, float dkd, float antiwindup);


float pid_process(struct sPID *pid, float error);

#endif
