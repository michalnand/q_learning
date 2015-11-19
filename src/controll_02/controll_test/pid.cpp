#include "pid.h"

void pid_init(struct sPID *pid, float kp, float ki, float kd, float antiwindup)
{
  pid->res = 0.0;
  pid->e0 = 0.0;
  pid->e1 = 0.0;
  pid->e2 = 0.0;

  pid->k1 = 0.0;
  pid->k2 = 0.0;
  pid->k3 = 0.0;

  pid->kp = 0.0;
  pid->ki = 0.0;
  pid->kd = 0.0;


  pid_set(pid, kp, ki, kd, antiwindup);
}

void pid_set(struct sPID *pid, float kp, float ki, float kd, float antiwindup)
{
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;

  pid->antiwindup = antiwindup;

  pid->k1 = pid->kp + pid->ki + pid->kd;
  pid->k2 = -pid->kp - 2.0*pid->kd;
  pid->k3 = pid->kd;
}

void pid_set_dif(struct sPID *pid, float dkp, float dki, float dkd, float antiwindup)
{
  pid->kp+= dkp;
  pid->ki+= dki;
  pid->kd+= dkd;

  if (pid->kp < 0.0)
    pid->kp = 0.0;

  if (pid->ki < 0.0)
    pid->ki = 0.0;

  if (pid->kd < 0.0)
    pid->kd = 0.0;

  pid->antiwindup = antiwindup;

  float limit = 2.0;

  if (pid->k1 > limit)
    pid->k1 = limit;

  if (pid->k1 < -limit)
    pid->k1 = -limit;

  if (pid->k2 > limit)
    pid->k2 = limit;

  if (pid->k2 < -limit)
    pid->k2 = -limit;

  if (pid->k3 > limit)
    pid->k3 = limit;

  if (pid->k3 < -limit)
    pid->k3 = -limit;

  pid->k1 = pid->kp + pid->ki + pid->kd;
  pid->k2 = -pid->kp - 2.0*pid->kd;
  pid->k3 = pid->kd;
}

float pid_process(struct sPID *pid, float error)
{
  pid->e2 = pid->e1;
  pid->e1 = pid->e0;
  pid->e0 = error;

  pid->res+= pid->e0*pid->k1 + pid->e1*pid->k2 + pid->e2*pid->k3;

  if (pid->res > pid->antiwindup)
    pid->res = pid->antiwindup;

  if (pid->res < -pid->antiwindup)
    pid->res = -pid->antiwindup;

  return pid->res;
}
