#ifndef _NANO_Q_H_
#define _NANO_Q_H_

#include "../common/common.h"

#define NANO_Q_ACTIONS_COUNT  ((u32)4)
#define NANO_Q_GAMMA_MAX      ((i32)64)

struct sNanoQ
{
  u32 action_id;
  i32 gamma;
  i32 q[NANO_Q_ACTIONS_COUNT];
};

void nano_q_init(struct sNanoQ *nq, i32 gamma);
void nano_q_select_action_id_best(struct sNanoQ *nq);
void nano_q_process(struct sNanoQ *nq, i32 reward);


#endif
