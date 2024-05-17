#include "QAM_Modem.hpp"
using namespace std;


vector<int> transmitt_bits(vector<int> &message, Modulator mod, double var)
{
    int len_mes = message.size();
    if (len_mes % 2 == 0 && len_mes % 4 == 0 && len_mes % 6 == 0)
    {
        // использование модулятора
        vector<vector<double>> modulated_signal = mod.modulate(message); //вектор с элементами I и Q (не собираются в комплексное число)
        // добавление шума
        AWGN noise(var);
        noise.add_noise(modulated_signal);
        // использование демодулятора
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

    // будем записывать данные для графика в файл
    ofstream file("bit_errors.txt");
    if (file.is_open())
    {
        file << "var,qam4,qam16,qam64"
             << "\n";
        for (auto var : var_values)
        {
            // смоделируем передачу сообщения из битов
            vector<int> result_qam4 = transmitt_bits(mes, Modulator(4), var); // вектор из битов на выходе
            vector<int> result_qam16 = transmitt_bits(mes, Modulator(16), var);
            vector<int> result_qam64 = transmitt_bits(mes, Modulator(64), var);
            // считаем количество ошибочно декодированных битов
            int biterr_qam4 = calculate_biterr(mes, result_qam4);
            int biterr_qam16 = calculate_biterr(mes, result_qam16);
            int biterr_qam64 = calculate_biterr(mes, result_qam64);
            // записываем в файл
            file << var << "," << ((double) biterr_qam4 / num_bits) << "," << ((double) biterr_qam16 / num_bits)
                 << "," << ((double) biterr_qam64 / num_bits) << "\n";
        }
        file.close();
    }
    else { cout << "Error - File is not opened" << "\n"; }
    return 0;
}