#ifndef _BASIS_FUNCTIONS_H_
#define _BASIS_FUNCTIONS_H_

#include "../../../common.h"


class CBasisFunctions
{
  private:
      u32 functions_count, dimension;
      float a_range, b_range, w_range;
      float **a, *b;

      float *w, *distance;

      std::vector<float> output, input;
      float linear_combination;

  public:
      CBasisFunctions(u32 count, u32 dimension, float a_range = 1.0, float b_range = 100.0, float w_range = 1.0);
      ~CBasisFunctions();

      void process(std::vector<float> input);
      void process_linear_combination(std::vector<float> input);

      std::vector<float> get();
      float get_linear_combination();

      void learn_linear_combination(float required_value, float learning_rate = 0.01, bool learn_basis = false);
      void save(u32 name_idx);


};

#endif
