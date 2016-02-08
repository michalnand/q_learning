#ifndef _WATER_H_
#define _WATER_H_

#include "kohonen_layer.h"

class CWater
{
    private:
        std::vector<std::vector<float>> input_data;

        class CKohonenLayer *knn;
        struct KNNLayerInitStructure knn_init;

        std::vector<float> output;

    public:
        CWater();
        ~CWater();

        void load_data();

        void log_input_data(char *file_name);

        void learn(u32 iterations);

        void process(char *file_name);


        void save_clusters(char *file_name);


};

#endif
