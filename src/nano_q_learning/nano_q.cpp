#include "nano_q.h"


void nano_q_init(struct sNanoQ *nq, i32 gamma)
{
  nq->gamma = gamma;

  u32 j;
  for (j = 0; j < NANO_Q_ACTIONS_COUNT; j++)
    nq->q[j] = 0;

  nq->action_id = 0;
}


void nano_q_select_action_id_best(struct sNanoQ *nq)
{
  if (nq->q[nq->action_id] >= 0)
    nq->action_id = nq->action_id;
  else
  {
    u32 tmp;
    do
    {
      tmp = rand()%NANO_Q_ACTIONS_COUNT;
    }
      while (tmp == nq->action_id);

      nq->action_id = tmp;
  }
}

void nano_q_process(struct sNanoQ *nq, i32 reward)
{
  u32 j, action_id_best = 0;
  for (j = 0; j < NANO_Q_ACTIONS_COUNT; j++)
    if (nq->q[j] > nq->q[action_id_best] )
      action_id_best = j;

  nq->q[nq->action_id] = NANO_Q_GAMMA_MAX*reward + (nq->gamma*nq->q[action_id_best])/NANO_Q_GAMMA_MAX;
}
