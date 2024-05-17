#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <stdexcept>
#include "helpful_functions.hpp"
using namespace std;

class Modulator
{
private:
    unsigned int order; // only 4, 16 or 64

    double assign_value(vector<int> symb) // нормировка сделана так, чтоб средняя мощность была равна 1
    {
        double sgn = (symb[0] == 1 ? 1 : -1);

        if (this->order == 4)
        {
            return sgn/sqrt(2);
        }
        else if (this->order == 16)
        {
            return (symb[1] == 0 ? 3 : 1) * sgn / sqrt(10);
        }
        else
        {
            return ((symb[1] == 0 ? (7 - 2 * symb[2]) : (1 + 2 * symb[2])) * sgn / sqrt(42));
        }
    }

public:
    Modulator(unsigned int order)
    {
        if (order == 4 || order == 16 || order == 64)
        {
            this->order = order;
        }
        else
        {
            throw invalid_argument("modulator order must be 4, 16 or 64");
        }
    }
    vector<vector<double>> modulate(vector<int> &message)
    {
        int m = int(log2(this->order)/2);

        vector<vector<double>> result(message.size() / (2 * m), vector<double>(2, 0));
        for (size_t i = 0; i < message.size() / (2 * m); i++)
        {
            result[i][0] = assign_value(vector<int>(message.begin() + 2 * m * i, message.begin() + 2 * m * i + m));
            result[i][1] = assign_value(vector<int>(message.begin() + 2 * m * i + m, message.begin() + 2 * m * i + 2 * m));
        }
        return result;
    }

    vector<int> get_QI_values() // will be needed for demodulation - вычисляет все возможные значения величины I(и соответсвтенно Q)
    {
        vector<int> result(sqrt(this->order), 0);
        int value = 1;
        for (size_t i = 0; i < result.size() / 2; i++)
        {
            result[2 * i] = value;
            result[2 * i + 1] = -value;
            value += 2;
        }
        return result;
    }
    unsigned int get_order() { return this->order; } // to get the order value
};


class AWGN
{
private:
    double var_2d; // дисперсия шума

public:
    AWGN(double var_2d = 1)
    {
        this->var_2d = var_2d;
    }
    void add_noise(vector<vector<double>> &signal)
    {
        default_random_engine generator;
        normal_distribution<double> distribution(0, sqrt(var_2d / 2));

        for (size_t i = 0; i < signal.size(); i++)
        {
            signal[i][0] += distribution(generator);
            // distribution.reset();
            signal[i][1] += distribution(generator);
        }
    }
};


class Demodulator
{
private:
    unsigned int order;
    vector<int> QI_values;

    int denoise_and_renorm_value(double value)
    {
        double norm = (this->order == 4 ? sqrt(2) : (this->order == 16 ? sqrt(10) : sqrt(42)));
        value *= norm;
        double min = abs(this->QI_values[0] - value);
        int denoised_val = QI_values[0];
        for (size_t i = 1; i < QI_values.size(); i++)
        {
            if (abs(this->QI_values[i] - value) < min)
            {
                min = abs(this->QI_values[i] - value);
                denoised_val = QI_values[i];
            }
        }
        return denoised_val;
    }

    vector<int> assign_bit(int value)
    {
        int bit_1 = (value > 0 ? 1 : 0);
        if (this->order == 4)
        {
            return {bit_1};
        }
        else if (this->order == 16)
        {
            int bit_2 = (abs(value) > 2 ? 0 : 1);
            return {bit_1, bit_2};
        }
        else
        {
            int bit_2 = abs(value) > 4 ? 0 : 1;
            int bit_3 = (int(abs(value)) % 7) > 2 ? 1 : 0;
            return {bit_1, bit_2, bit_3};
        }
    }

    vector <vector<int>> denoise_signal(vector<vector<double>> &signal)
    {
        vector <vector<int>> denoised_signal(signal.size(), vector<int>(2, 0));
        for (size_t i = 0; i < signal.size(); i++)
        {
            denoised_signal[i][0] = denoise_and_renorm_value(signal[i][0]);
            denoised_signal[i][1] = denoise_and_renorm_value(signal[i][1]);
        }
        return denoised_signal;
    }

public:
    Demodulator(Modulator mod)
    {
        this->order = mod.get_order();
        this->QI_values = mod.get_QI_values();
    }

    vector<int> demodulate(vector<vector<double>> &signal)
    {
        vector <vector<int>> denoised_sig = denoise_signal(signal);
        unsigned int m = int(log2(this->order)/2);
        vector<int> result(2 * m * denoised_sig.size(), 0);
        vector<int> tempQ;
        vector<int> tempI;
        for (size_t i = 0; i < denoised_sig.size(); i++)
        {
            tempI = assign_bit(denoised_sig[i][0]);
            tempQ = assign_bit(denoised_sig[i][1]);
            for (size_t k = 0; k < m; k++)
            {
                result[2 * m * i + k] = tempI[k];
                result[2 * m * i + m + k] = tempQ[k];
            }
        }
        return result;
    }
};