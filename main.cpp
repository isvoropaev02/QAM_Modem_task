#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <stdexcept>
#include "QAM_Modem.hpp"
using namespace std;


vector<int> transmitt_bits(vector<int> &message, Modulator mod, double var)
{
    int len_mes = message.size();
    if (len_mes % 2 == 0 && len_mes % 4 == 0 && len_mes % 6 == 0)
    {
        vector<vector<double>> modulated_signal = mod.modulate(message);
        //print_qam_vector(modulated_signal);
        AWGN noise(var);
        noise.add_noise(modulated_signal);
        Demodulator dem(mod);
        return dem.demodulate(modulated_signal);
    }
    else
    {
        throw invalid_argument("size of message must a multiple of 2, 4 and 6, for ex. 12, 24, 48...");
    }
}


int main()
{
    int num_bits = 1200; // сколько бит передавать
    vector <double> var_values = linspace(0.01, 2.0, 400); // значения дисперсии шума
    
    // сначала сгенерируем сообщение и посмотрим его
    vector<int> mes = generate_bit_message(num_bits);
    //print_bits_vector(mes);

    ofstream file("bit_errors.txt");
    if (file.is_open())
    {
        file << "var,qam4,qam16,qam64"
             << "\n";
        for (auto var : var_values)
        {
            // смоделируем передачу сообщения из битов
            vector<int> result_qam4 = transmitt_bits(mes, Modulator(4), var);
            vector<int> result_qam16 = transmitt_bits(mes, Modulator(16), var);
            vector<int> result_qam64 = transmitt_bits(mes, Modulator(64), var);
            // считаем количество ошибочно декодированных битов
            int biterr_qam4 = calculate_biterr(mes, result_qam4);
            int biterr_qam16 = calculate_biterr(mes, result_qam16);
            int biterr_qam64 = calculate_biterr(mes, result_qam64);

            file << var << "," << ((double) biterr_qam4 / num_bits) << "," << ((double) biterr_qam16 / num_bits)
                 << "," << ((double) biterr_qam64 / num_bits) << "\n";
        }
        file.close();
    }
    else
    {
        cout << "Error - File is not opened"
             << "\n";
    }

    // cout << "QAM4" <<"\n";
    // cout << "biterr=" << calculate_biterr(mes, result_qam4) << "\n";
    // print_bits_vector(result_qam4);
    // cout << "QAM16" <<"\n";
    // cout << "biterr=" << calculate_biterr(mes, result_qam16) << "\n";
    // print_bits_vector(result_qam16);
    // cout << "QAM64" <<"\n";
    // cout << "biterr=" << calculate_biterr(mes, result_qam64) << "\n";
    // print_bits_vector(result_qam64);


    // посмотреть точки на созвездии модулятора
    // Modulator mod(16);
    // vector<vector<double>> modulated_signal = mod.modulate(mes);
    // AWGN noise(0.005);
    // noise.add_noise(modulated_signal);
    // ofstream file2("signal_on_constellation.txt");
    // if (file2.is_open())
    // {
    //     file2 << "I,Q"
    //          << "\n";
    //     for (size_t i = 0; i < modulated_signal.size(); i++)
    //     {
    //         file2 << modulated_signal[i][0] << "," << modulated_signal[i][1] << "\n";
    //     }
    //     file2.close();
    // }
    // else
    // {
    //     cout << "Error - File is not opened"
    //          << "\n";
    // }
    
    return 0;
}