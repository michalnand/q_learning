#ifndef _BASIS_FUNCTIONS_HYBRID_H_
#define _BASIS_FUNCTIONS_HYBRID_H_

#include "../../../common.h"


class CBasisFunctionsHybrid
{
  private:
    u32 count_peek, count_hill, dimension;

    std::vector<float> input;

    float *peak_value, **peak_centre;

    float *hill_distance, *hill_value;
    float **hill_centre, *hill_shape, *hill_output;

    float output;

    u32 peak_min_dist_idx, hill_min_dist_idx;
    float peak_min_dist;

    float peak_min_dist_value;
    float hill_min_dist_value;

  public:
    CBasisFunctionsHybrid(u32 count_peek, u32 count_hill, u32 dimension);
    ~CBasisFunctionsHybrid();

    void process(std::vector<float> input);
    float get();
    void learn(float required_value, float learning_rate = 0.01);
};

#endif
