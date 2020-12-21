#include "wav.h"

void getWavHeader(FILE *file, WAVHEADER* header) {
    fread(header, sizeof(WAVHEADER), 1, file);
}

void printWavData(WAVHEADER* header) {
    // Выводим полученные данные
    std::cout << header->chunkId[0] << header->chunkId[1] << header->chunkId[2] << header->chunkId[3] << std::endl;
    printf("Chunk size: %d\n", header->chunkSize);
    std::cout << header->format[0] << header->format[1] << header->format[2] << header->format[3] << std::endl;
    std::cout << header->subchunk1Id[0] << header->subchunk1Id[1] << header->subchunk1Id[2] << header->subchunk1Id[3]
              << std::endl;
    printf("SubChunkId1: %d\n", header->subchunk1Size);
    printf("Audio format: %d\n", header->audioFormat);
    printf("Channels: %d\n", header->numChannels);
    printf("Sample rate: %d\n", header->sampleRate);
    printf("Bits per sample: %d\n", header->bitsPerSample);
    std::cout << header->subchunk2Id[0] << header->subchunk2Id[1] << header->subchunk2Id[2] << header->subchunk2Id[3]
              << std::endl;

    // Посчитаем длительность воспроизведения в секундах
    float fDurationSeconds = 1.f * header->subchunk2Size / (header->bitsPerSample / 8) / header->numChannels / header->sampleRate;
    int iDurationMinutes = (int) floor(fDurationSeconds) / 60;
    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
    printf("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);

}

void saveNewWav(std::string result, WAVHEADER* header, std::vector<std::complex<double>>& data) {

    WAVHEADER new_header = *header;
    new_header.subchunk2Size = data.size();

    char *new_data = new char[data.size()];
    for (size_t i = 0; i < data.size(); ++i) {
        new_data[i] = static_cast<char>(data[i].real());
    }

    FILE* result_file = fopen(result.c_str(), "wb");
    fwrite(&new_header, sizeof(new_header), 1, result_file);
    fwrite(new_data, 1, new_header.subchunk2Size, result_file);

    delete[] new_data;

}
