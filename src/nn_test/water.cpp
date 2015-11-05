#include "water.h"
#include "log.h"
#include "reader.h"

CWater::CWater()
{
    load_data();

    printf("loading done\n");

    knn_init.neurons_count = 8;
    knn_init.inputs_count = input_data[0].size();
    knn_init.learning_constant = 1.0/100.0;
    knn_init.output_limit = 1.0;


    knn = new CKohonenLayer(knn_init);

    u32 i;

    for (i = 0; i < knn_init.neurons_count; i++)
        output.push_back(0.0);
}

CWater::~CWater()
{
    delete knn;
}

void CWater::load_data()
{
    u32 k;
    u32 files_count = 68;

    for (k = 1; k <= files_count; k++)
    {
        char f_name[1024];
        sprintf(f_name,"water_results/spektra_data/spektrum%u.txt", k);

        std::string fname = f_name;
        std::vector<uint32_t> columns;
        columns.push_back(0);
        columns.push_back(1);

        printf("loading file %s\n", fname.c_str());

        class CReader *reader;
        reader = new CReader(fname, columns);

        reader->read();

        input_data.push_back(reader->get()[1]);

        delete reader;
    }

    //normalise input into <-1, 1>

    u32 j, i;
    float max = -99999999999999999999.9;
    float min = -max;

    for (j = 0; j < input_data.size(); j++)
        for (i = 0; i < input_data[j].size(); i++)
        {
            if (input_data[j][i] > max)
                max = input_data[j][i];
            if (input_data[j][i] < min)
                min = input_data[j][i];
        }

    double k_ = 2.0/(max - min);
    double q_ = 1.0 - k_*max;

    for (j = 0; j < input_data.size(); j++)
        for (i = 0; i < input_data[j].size(); i++)
        {
            input_data[j][i] = k_*input_data[j][i] + q_;
        }
}

void CWater::log_input_data(char *file_name)
{
    u32 i, j;

    class CLog *log;

    log = new CLog(file_name, input_data.size());

    for (i = 0; i < input_data[0].size(); i++)
    {
        for (j = 0; j < input_data.size(); j++)
            log->add(j, input_data[j][i]);
    }

    log->save();
    delete log;
}

void CWater::learn(u32 iterations)
{
    u32 j, k;

    float a = 0.95;

    for (k = 0; k < iterations; k++)
    {
        for (j = 0; j < input_data.size(); j++)
        {
            knn->process(input_data[j]);
            knn->learn();

            u32 winning_neuron_idx = knn->get_output_winning_neuron_idx();
            output[winning_neuron_idx] = (1.0 - a)*output[winning_neuron_idx] + a*j;
        }

        printf("learning %6.3f%% done\n", (100.0*k)/iterations);
    }
}

void CWater::process(char *file_name)
{
    u32 j, i;

    class CLog *log;

    log = new CLog(file_name, 4);

    for (j = 0; j < input_data.size(); j++)
    {
        knn->process(input_data[j]);
        u32 winning_neuron_idx = knn->get_output_winning_neuron_idx();

        std::vector<float> knn_output = knn->get_output();


        float max = -9999999999999999999999.99;
        float min = -max;

        for (i = 0; i < knn_output.size(); i++)
        {
            if (max < knn_output[i])
                max = knn_output[i];
            if (min > knn_output[i])
                min = knn_output[i];
        }

        double k_ = 1.0/(max - min);
        double q_ = 1.0 - k_*max;

        for (i = 0; i < knn_output.size(); i++)
        {
            knn_output[i] = knn_output[i]*k_ + q_;
        }

        for (i = 0; i < knn_output.size(); i++)
            knn_output[i] = pow(10000.0, knn_output[i]);

        float sum = 0.0;
        for (i = 0; i < knn_output.size(); i++)
            sum+= knn_output[i];

        float res = 0.0;
        for (i = 0; i < knn_output.size(); i++)
            res+= (knn_output[i]/sum)*output[i];


        log->add(0, winning_neuron_idx);
        log->add(1, 500.0*knn->get_output_winning_distance());
        log->add(2, output[winning_neuron_idx]);
        log->add(3, res);
    }

    log->save();
}

void CWater::save_clusters(char *file_name)
{
    u32 i, j;
    class CLog *log;

    log = new CLog(file_name, knn_init.neurons_count);

    float **w;

    w = knn->get_weights();

    for (i = 0; i < knn_init.inputs_count; i++)
        for (j = 0; j < knn_init.neurons_count; j++)
            log->add(j, w[j][i]);

    log->save();
    delete log;
}
