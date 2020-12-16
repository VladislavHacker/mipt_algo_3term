#include <cmath>
#include <complex>
#include <cstdio>
#include <iostream>
#include <vector>

void fftStraight(std::vector<std::complex<double>>& data);

void fftReversed(std::vector<std::complex<double>>& data);

void commpressData(std::vector<std::complex<double>>& data, char percents = 20);
