//#include "QAM_Modem.hpp"
#include "QAM_modem_complex.hpp"
#include <complex>
using namespace std;

int main()
{
    Modulator qammod(64);
    Demodulator qamdemod(qammod);
    vector<int> bits = generate_bit_message(12);
    cout << "Initial bits sequence:" << "\n";
    print_bits_vector(bits);
    vector<complex<double>> signal = qammod.modulate(bits);
    cout << "Modulator output signal:" << "\n";
    print_complex_vector(signal);
    AWGN noise(0.1);
    noise.add_noise(signal);
    cout << "Signal with additive noise:" << "\n";
    print_complex_vector(signal);
    vector<int> decoded_bits = qamdemod.demodulate(signal);
    cout << "Bits sequence reconstructed from signal with noise:" << "\n";
    print_bits_vector(decoded_bits);

    return 0;
}
