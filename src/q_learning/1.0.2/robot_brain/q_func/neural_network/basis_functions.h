#ifndef _BASIS_FUNCTIONS_H_
#define _BASIS_FUNCTIONS_H_

#include "../../../common.h"

#define BASIS_FUNCTION_TYPE_GAUSS        (u32)(1)
#define BASIS_FUNCTION_TYPE_KOHONEN      (u32)(2)
#define BASIS_FUNCTION_TYPE_GAUSS_PEAK   (u32)(3)

class CBasisFunctions
{
  private:
      u32 functions_count, dimension;
      float a_range, b_range, w_range;
      float **a, *b, *h;

      u32 v_size;
      float *w, *distance;

      std::vector<float> output, input, output_h;
      float linear_combination;

      u32 function_type;

  public:
      CBasisFunctions(u32 count, u32 dimension, float a_range = 1.0, float b_range = 100.0, float w_range = 1.0, u32 function_type = BASIS_FUNCTION_TYPE_GAUSS);
      ~CBasisFunctions();

      void process(std::vector<float> input);
      void process_linear_combination(std::vector<float> input);

      std::vector<float> get();
      float get_linear_combination();

      void learn_linear_combination(float required_value, float learning_rate = 0.01, bool learn_basis = false, float reward_value = 0.0);
      void save(u32 name_idx);


};

#endif
