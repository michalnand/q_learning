#ifndef _NN_TEST_H_
#define _NN_TEST_H_

#include "nn.h"
#include "kohonen_layer.h"

struct sNNTestResult
{
    float error_average, error_max, error_min;
};

class CNNTest
{
    private:
        struct sNNTestResult test_result, experiment_result;

    public:
        CNNTest();
        ~CNNTest();

        void process();

        void process_test(char *result_path, u32 id);
        void process_single_test(u32 id, bool log_enabled, char *log_path);

    private:


        float f_and(float x0, float x1);
        float f_or(float x0, float x1);
        float f_not(float x0);

        std::vector<float> get_input(float x0, float x1);
        std::vector<float> get_required_output(std::vector<float> input, u32 id);

        float rnd();
};

#endif
