#include "videoCoder.h"
#include <opencv2/opencv.hpp>
#include <algorithm>

// construtor videocoder: responsável por inicializar os ficheiros de entrada e saída, 
// assim como definir o modo (encode ou decode).
VideoCoder::VideoCoder(const std::string &inputFile, const std::string &outputFile, bool mode)
    : input(inputFile), output(outputFile), encodeMode(mode) {}

// funct codificar vídeo.
// abre um input video file, aplica predicter e golomb nos resíduos 
// e grava o resultado num ficheiro binário (output.bin)
// heard: dimensões do vídeo e processa cada frame e canal de cor.
void VideoCoder::encodeVideo() {
    cv::VideoCapture cap(input); // abrir video
    if (!cap.isOpened()) throw std::runtime_error("Erro ao abrir o input video file.");

    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)); // largura dos frames
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)); // altura

    BitStream bitStream;
    bitStream.openFile(output, std::ios::out | std::ios::binary);

    bitStream.writeBits(frameWidth, 16);
    bitStream.writeBits(frameHeight, 16);

    Golomb golomb(4);
    cv::Mat frame;
    while (cap.read(frame)) {
        for (int row = 0; row < frame.rows; ++row) {
            for (int col = 0; col < frame.cols; ++col) {
                for (int channel = 0; channel < 3; ++channel) {
                    int a = 0, b = 0, c = 0; 
                    int prediction = Predictors().predictJPEG_LS(a, b, c);
                    int residual = frame.at<cv::Vec3b>(row, col)[channel] - prediction;
                    golomb.encode(bitStream, residual);
                }
            }
        }
    }
    bitStream.flushBuffer(); // gravar
}

// descodifica vídeo com golomb e predição
void VideoCoder::decodeVideo() {
    BitStream bitStream;
    bitStream.openFile(input, std::ios::in | std::ios::binary);

    int frameWidth = bitStream.readBits(16);
    int frameHeight = bitStream.readBits(16);

    Golomb golomb(4);

    cv::VideoWriter writer(output, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 30, cv::Size(frameWidth, frameHeight), true);
    if (!writer.isOpened()) throw std::runtime_error("Erro ao abrir o output video file.");

    cv::Mat frame(frameHeight, frameWidth, CV_8UC3);
    while (!bitStream.isEndOfFile()) {
        for (int row = 0; row < frame.rows; ++row) {
            for (int col = 0; col < frame.cols; ++col) {
                for (int channel = 0; channel < 3; ++channel) {
                    int a = 0, b = 0, c = 0;
                    int prediction = Predictors().predictJPEG_LS(a, b, c);
                    int residual = golomb.decode(bitStream);
                    frame.at<cv::Vec3b>(row, col)[channel] = std::clamp(prediction + residual, 0, 255);
                }
            }
        }
        writer.write(frame);
    }
}