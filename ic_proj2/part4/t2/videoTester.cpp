#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

// Helper function to calculate MSE between two images
double calculateMSEBetween(const cv::Mat &img1, const cv::Mat &img2) {
    if (img1.empty() || img2.empty() || img1.size() != img2.size() || img1.type() != img2.type()) {
        cerr << "Error: Images are either empty or have different sizes/types." << endl;
        return -1.0;
    }

    double mse = 0.0;
    for (int y = 0; y < img1.rows; y++) {
        for (int x = 0; x < img1.cols; x++) {
            for (int c = 0; c < img1.channels(); c++) {
                double diff = img1.at<Vec3b>(y, x)[c] - img2.at<Vec3b>(y, x)[c];
                mse += diff * diff;
            }
        }
    }
    mse /= (double)(img1.total() * img1.channels());
    return mse;
}

// Helper function to calculate PSNR between two images
double calculatePSNRBetween(const cv::Mat &img1, const cv::Mat &img2) {
    double mse = calculateMSEBetween(img1, img2);
    if (mse <= 0) {
        return 100.0;  // Identical images
    }

    double psnr = 10.0 * log10((255 * 255) / mse);
    return psnr;
}

// Main function to compare two videos
void compareVideos(const string &originalPath, const string &reconstructedPath) {
    VideoCapture originalVideo(originalPath);
    VideoCapture reconstructedVideo(reconstructedPath);

    if (!originalVideo.isOpened() || !reconstructedVideo.isOpened()) {
        cerr << "Error: Cannot open video files for comparison." << endl;
        return;
    }

    Mat originalFrame, reconstructedFrame;
    int frameCount = 0;
    double totalMSE = 0.0;
    double totalPSNR = 0.0;

    while (true) {
        originalVideo >> originalFrame;
        reconstructedVideo >> reconstructedFrame;

        if (originalFrame.empty() || reconstructedFrame.empty()) {
            break;  // End of one of the videos
        }

        // Check dimensions
        if (originalFrame.size() != reconstructedFrame.size() || originalFrame.type() != reconstructedFrame.type()) {
            cerr << "Error: Frame size or type mismatch at frame " << frameCount << endl;
            return;
        }

        double mse = calculateMSEBetween(originalFrame, reconstructedFrame);
        double psnr = calculatePSNRBetween(originalFrame, reconstructedFrame);

        totalMSE += mse;
        totalPSNR += psnr;
        frameCount++;

        cout << "Frame " << frameCount << ": MSE = " << mse << ", PSNR = " << psnr << " dB" << endl;
    }

    if (frameCount > 0) {
        cout << "\n=== Summary ===" << endl;
        cout << "Total frames compared: " << frameCount << endl;
        cout << "Average MSE: " << (totalMSE / frameCount) << endl;
        cout << "Average PSNR: " << (totalPSNR / frameCount) << " dB" << endl;
    } else {
        cerr << "Error: No frames were compared." << endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./compare <original_video> <reconstructed_video>" << endl;
        return 1;
    }

    string originalPath = argv[1];
    string reconstructedPath = argv[2];

    compareVideos(originalPath, reconstructedPath);

    return 0;
}
