#include <cassert>
#include <cmath>
#include <complex>
#include <cstdio>
#include <iostream>
#include <vector>

#include "fft.h"
#include "wav.h"

int main(int argc, char** argv) {

    if (argc != 3) {
       std::cout << "WRONG ARGUMENTS, try: vhWawCompressor source.waw result.waw" << std::endl;
       return 0;
    }

    std::string source(argv[1]);
    std::string result(argv[2]);

    FILE *file = fopen(source.c_str(), "rb");
    if (!file) {
        perror("Failed open file");
        return 0;
    }

    WAVHEADER header;

    getWavHeader(file, &header);

    printWavData(&header);
    char *data = new char[header.subchunk2Size];
    fread(data, header.subchunk2Size, 1, file);

    std::cout << "Data is successfully loaded." << std::endl;

    std::vector<std::complex<double>> complex_data(header.subchunk2Size);
    for (size_t i = 0; i < header.subchunk2Size; ++i) {
        complex_data[i] = data[i];
    }

    size_t tmp = 1;
    while (tmp < header.subchunk2Size) {
        tmp <<= 1;
    }

    complex_data.resize(tmp);

    commpressData(complex_data);

    saveNewWav(result, &header, complex_data);

    delete[] data;
    fclose(file);

    return 0;
}
