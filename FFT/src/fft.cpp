#include <cmath>
#include <complex>
#include <cstdio>
#include <iostream>
#include <vector>

std::vector<std::complex<double>> getHalfData(std::vector<std::complex<double>>& data, bool is_odd) {
    std::vector<std::complex<double>> res;
    for (size_t i = !is_odd; i < data.size(); i+= 2) {
        res.push_back(data[i]);
    }
    return res;
}

void fft(std::vector<std::complex<double>> &data, bool reversed) {
    if (data.size() == 1) {
        return;
    }
    std::vector<std::complex<double>> data_odd = getHalfData(data, false);
    std::vector<std::complex<double>> data_even = getHalfData(data, true);

    fft(data_odd, reversed);
    fft(data_even, reversed);

    double angle = 1;
    if (reversed) {
        angle = -1;
    }
    size_t n = data.size();
    angle *= 2.0 * M_PI / n;

    std::complex<double> w_cur(1);
    std::complex<double> w_step(std::cos(angle), std::sin(angle));

    for (size_t i = 0; i < n / 2; ++i) {
        data[i] = data_odd[i] + w_cur * data_even[i];
        data[i + n / 2] = data_odd[i] - w_cur * data_even[i];
        if (reversed) {
            data[i] /= 2;
            data[i + n / 2] /= 2;
        }
        w_cur *= w_step;
    }

}

void fftStraight(std::vector<std::complex<double>>& data) {
    fft(data, false);
}
void fftReversed(std::vector<std::complex<double>>& data) {
    fft(data, true);
}

void commpressData(std::vector<std::complex<double>>& data, char percents) {
    fftStraight(data);
    size_t zero_start = data.size() / 4;
    for(size_t i = zero_start; i < data.size(); i++){
        data[i] = 0;
    }
    fftReversed(data);
}
