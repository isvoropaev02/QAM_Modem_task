#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <stdexcept>
using namespace std;

template <typename Type>
void print_qam_vector(vector<vector<Type>> &result_qam) // может быть double, а при демодуляции в один момент может быть int
{
    for (size_t i = 0; i < result_qam.size(); i++)
    {
        cout << result_qam[i][0] << (result_qam[i][1] >= 0 ? "+" : "") << result_qam[i][1] << "j" << " ";
    }
    cout << "\n";
}

void print_bits_vector(vector<int> &mes)
{
    for (size_t i = 0; i < mes.size(); i++)
    {
        cout << mes[i]  << " ";
    }
    cout << "\n";
}

vector<int> generate_bit_message(int size)
{
    vector<int> mes(size, 0);
    random_device rd;  // Will be used to obtain a seed for the random number engine
    minstd_rand0 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<double> dis(-1, 1);
    for (size_t i = 0; i < mes.size(); i++)
    {
        mes[i] = int(dis(gen) >= 0 ? 1 : 0);
    }
    return mes;
}


int calculate_biterr(vector<int> &initial_bits, vector<int> &recieved_bits)
{
    int biterr = 0;
    for (size_t i = 0; i < initial_bits.size(); i++)
    {
        biterr += int(abs(initial_bits[i]-recieved_bits[i]));
    }
    return biterr;
}


vector <double> linspace(double min_, double max_, unsigned int num_points)
{
    vector<double> result(num_points, 0);
    for (size_t i = 0; i < num_points; i++)
    {
        result[i] = min_ + (max_ - min_)/ num_points * i;
    }
    return result;
}

