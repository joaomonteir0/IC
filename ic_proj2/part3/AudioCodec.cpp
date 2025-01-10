#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <ctime>

const std::vector<std::string> AUDIO_FILES = {"sample01.wav"};
const std::vector<int> FIXED_M_VALUES = {128, 256, 512, 1024, 2048};
const std::vector<int> BS_VALUES = {128, 512, 1024, 2048, 16384};
const std::vector<int> QUANT_VALUES = {2, 4, 6, 8};

void dump_stats(std::ofstream &file, const std::vector<std::string> &times, const std::vector<std::string> &sizes) {
    file << "times:\n";
    for (const auto &t : times) file << t << "\n";

    file << "sizes:\n";
    for (const auto &s : sizes) file << s << "\n";
}

int lossy_encoder(std::ofstream &loe, int idx) {
    std::vector<std::string> times;
    std::vector<std::string> sizes;

    loe << "Testing with auto M different block 2048\n";
    loe << "Testing quantization values (2, 4, 6, 8)\n";

    for (const auto &audio : AUDIO_FILES) {
        loe << audio << "\n";
        for (const auto &q : QUANT_VALUES) {
            std::stringstream cmd_time, cmd_size;
            cmd_time << "./GolombEncoder " << audio << " " << idx << " 2048 auto " << q;
            cmd_size << "ls -l " << idx;

            char buffer[128];
            FILE* pipe = popen(cmd_time.str().c_str(), "r");
            fgets(buffer, 128, pipe);
            times.push_back(std::string(buffer));
            pclose(pipe);

            pipe = popen(cmd_size.str().c_str(), "r");
            fgets(buffer, 128, pipe);
            sizes.push_back(std::string(buffer));
            pclose(pipe);

            idx++;
        }
        dump_stats(loe, times, sizes);
        times.clear();
        sizes.clear();
    }
    return idx;
}

int lossless_encoder(std::ofstream &lle, int idx) {
    std::vector<std::string> times;
    std::vector<std::string> sizes;

    lle << "Testing with fixed M values (128, 256, 512, 1024, 2048)\n";
    for (const auto &audio : AUDIO_FILES) {
        lle << audio << "\n";
        for (const auto &m : FIXED_M_VALUES) {
            std::stringstream cmd_time, cmd_size;
            cmd_time << "./GolombEncoder " << audio << " " << idx << " " << m;
            cmd_size << "ls -l " << idx;

            char buffer[128];
            FILE* pipe = popen(cmd_time.str().c_str(), "r");
            fgets(buffer, 128, pipe);
            times.push_back(std::string(buffer));
            pclose(pipe);

            pipe = popen(cmd_size.str().c_str(), "r");
            fgets(buffer, 128, pipe);
            sizes.push_back(std::string(buffer));
            pclose(pipe);

            idx++;
        }
        dump_stats(lle, times, sizes);
        times.clear();
        sizes.clear();
    }

    lle << "\nAuto M and different block sizes (128, 512, 1024, 2048, 16384)\n";
    for (const auto &audio : AUDIO_FILES) {
        lle << audio << "\n";
        for (const auto &bs : BS_VALUES) {
            std::stringstream cmd_time, cmd_size;
            cmd_time << "./GolombEncoder " << audio << " " << idx << " " << bs << " auto";
            cmd_size << "ls -l " << idx;

            char buffer[128];
            FILE* pipe = popen(cmd_time.str().c_str(), "r");
            fgets(buffer, 128, pipe);
            times.push_back(std::string(buffer));
            pclose(pipe);

            pipe = popen(cmd_size.str().c_str(), "r");
            fgets(buffer, 128, pipe);
            sizes.push_back(std::string(buffer));
            pclose(pipe);

            idx++;
        }
        dump_stats(lle, times, sizes);
        times.clear();
        sizes.clear();
    }
    return idx;
}

int decoder(std::ofstream &d, int idx) {
    for (int i = 0; i < idx; ++i) {
        std::stringstream cmd_decode;
        cmd_decode << "./GolombDecoder " << i << " " << i << ".wav";

        char buffer[128];
        FILE* pipe = popen(cmd_decode.str().c_str(), "r");
        fgets(buffer, 128, pipe);
        d << std::string(buffer) << "\n";
        pclose(pipe);

        std::stringstream cmd_rm1, cmd_rm2;
        cmd_rm1 << "rm " << i;
        cmd_rm2 << "rm " << i << ".wav";
        system(cmd_rm1.str().c_str());
        system(cmd_rm2.str().c_str());
    }
    return idx;
}

int main() {
    std::ofstream lle("lossless_encoder.txt");
    std::ofstream loe("lossy_encoder.txt");
    std::ofstream d("decoder.txt");

    lle << "[Lossless Encoder]\n\n";
    std::cout << "Testing Lossless Encoder\n";
    int idx = lossless_encoder(lle, 0);

    loe << "[Lossy Encoder]\n\n";
    std::cout << "Testing Lossy Encoder\n";
    idx = lossy_encoder(loe, idx);

    d << "[Testing Decoder]\n\n";
    std::cout << "Testing Decoder\n";
    idx = decoder(d, idx);

    lle.close();
    loe.close();
    d.close();

    return 0;
}
