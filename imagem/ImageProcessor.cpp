#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>  // For log10
#include <cstdio>  // For popen and pclose

class ImageProcessor {
private:
    cv::Mat img;   // Single image (for filtering)
    cv::Mat img1;  // First image (for comparison)
    cv::Mat img2;  // Second image (for comparison)

public:
    // Function to load a single image
    bool loadImage(const std::string &imagePath) {
        img = cv::imread(imagePath);  // Load the image using OpenCV
        if (img.empty()) {
            std::cerr << "Error: Image is empty." << std::endl;
            return false;
        }
        return true;
    }

    // Function to load two images for comparison
    bool loadImages(const std::string &imagePath1, const std::string &imagePath2) {
        img1 = cv::imread(imagePath1);  // Load the first image
        img2 = cv::imread(imagePath2);  // Load the second image

        if (img1.empty()) {
            std::cerr << "Error: First image is empty." << std::endl;
            return false;
        }
        if (img2.empty()) {
            std::cerr << "Error: Second image is empty." << std::endl;
            return false;
        }
        if (img1.size() != img2.size() || img1.type() != img2.type()) {
            std::cerr << "Error: Images must have the same size and type." << std::endl;
            return false;
        }
        return true;
    }

    // Function to display the original loaded image
    void displayOriginalImage() {
        if (img.empty()) {
            std::cerr << "Error: No image loaded." << std::endl;
            return;
        }
        cv::imshow("Original Image", img);
        cv::waitKey(0);  // Wait for user to close the window
    }

    // Function to display the image channels and grayscale
    void displayImageChannels() {
        if (img.empty()) {
            std::cerr << "Error: No image loaded." << std::endl;
            return;
        }

        if (img.channels() == 3) {
            std::vector<cv::Mat> channels;
            cv::split(img, channels);

            cv::imshow("Red Channel", channels[2]);
            cv::imshow("Green Channel", channels[1]);
            cv::imshow("Blue Channel", channels[0]);
        } else {
            std::cerr << "The image does not have multiple channels." << std::endl;
        }

        cv::Mat grayscale;
        cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);
        cv::imshow("Grayscale Image", grayscale);
        cv::waitKey(0);  // Wait for user to close the window
    }

    // Function to calculate the histogram of a grayscale image
    std::vector<int> calculateHistogram() {
        if (img.empty()) {
            std::cerr << "Error: No image loaded." << std::endl;
            return std::vector<int>(256, 0);
        }

        // Convert to grayscale if not already
        cv::Mat grayscale;
        if (img.channels() == 3) {
            cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);
        } else {
            grayscale = img.clone();
        }

        // Initialize the histogram (256 bins for intensities 0-255)
        std::vector<int> histogram(256, 0);

        // Calculate the histogram
        for (int y = 0; y < grayscale.rows; y++) {
            for (int x = 0; x < grayscale.cols; x++) {
                int intensity = grayscale.at<uchar>(y, x);
                histogram[intensity]++;
            }
        }

        return histogram;
    }

    // Function to plot the histogram using Gnuplot via a pipe
    void plotHistogramWithGnuplot(const std::vector<int>& histogram) {
        FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
        if (!gnuplotPipe) {
            std::cerr << "Error: Could not open pipe to Gnuplot." << std::endl;
            return;
        }

        // Write Gnuplot commands directly to the pipe
        fprintf(gnuplotPipe, "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n");
        fprintf(gnuplotPipe, "set output 'histogram.png'\n");
        fprintf(gnuplotPipe, "set title 'Grayscale Histogram'\n");
        fprintf(gnuplotPipe, "set xlabel 'Intensity'\n");
        fprintf(gnuplotPipe, "set ylabel 'Frequency'\n");
        fprintf(gnuplotPipe, "set boxwidth 0.9 relative\n");
        fprintf(gnuplotPipe, "set style fill solid 1.0\n");
        fprintf(gnuplotPipe, "set yrange [0:*]\n");
        fprintf(gnuplotPipe, "plot '-' using 1:2 with boxes lc rgb 'blue' notitle\n");

        // Send the histogram data to Gnuplot
        for (int i = 0; i < 256; ++i) {
            fprintf(gnuplotPipe, "%d %d\n", i, histogram[i]);
        }

        // Tell Gnuplot that the data is finished
        fprintf(gnuplotPipe, "e\n");

        // Close the Gnuplot pipe
        pclose(gnuplotPipe);

        std::cout << "Histogram generated: histogram.png" << std::endl;
    }

    // Function to generate the histogram
    void plotHistogramImage() {
        std::vector<int> histogram = calculateHistogram();
        plotHistogramWithGnuplot(histogram);
    }

    // Function to apply Gaussian filter and display original and blurred image
    void applyGaussianFilter(int kernelSize) {
        if (img.empty()) {
            std::cerr << "Error: No image loaded." << std::endl;
            return;
        }

        if (kernelSize % 2 == 0) {
            kernelSize += 1;  // Ensure kernel size is odd
        }
        if (kernelSize < 3) {
            kernelSize = 3;  // Minimum kernel size
        }

        cv::Mat blurredImage;
        cv::GaussianBlur(img, blurredImage, cv::Size(kernelSize, kernelSize), 0);

        // Display original and blurred images
        cv::imshow("Original Image", img);
        cv::imshow("Gaussian Blurred Image", blurredImage);
        cv::waitKey(0);

        // Save the blurred image
        std::string outputFile = "blurred_image.png";
        cv::imwrite(outputFile, blurredImage);
        std::cout << "Blurred image saved: " << outputFile << std::endl;
    }

    // Function to calculate the Mean Squared Error (MSE)
    double calculateMSE() {
        if (img1.empty() || img2.empty()) {
            std::cerr << "Error: One or both images are not loaded." << std::endl;
            return -1.0;
        }

        double mse = 0.0;
        for (int y = 0; y < img1.rows; y++) {
            for (int x = 0; x < img1.cols; x++) {
                for (int c = 0; c < img1.channels(); c++) {
                    double diff = img1.at<cv::Vec3b>(y, x)[c] - img2.at<cv::Vec3b>(y, x)[c];
                    mse += diff * diff;
                }
            }
        }
        mse /= (double)(img1.total() * img1.channels());
        return mse;
    }

    // Function to calculate the Peak Signal-to-Noise Ratio (PSNR)
    double calculatePSNR() {
        double mse = calculateMSE();
        if (mse == 0) {
            return 100.0;  // Identical images
        }

        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }

    // Function to display the difference image
    void displayDifferenceImage() {
        if (img1.empty() || img2.empty()) {
            std::cerr << "Error: One or both images are not loaded." << std::endl;
            return;
        }

        cv::Mat diff;
        cv::absdiff(img1, img2, diff);  // Calculate absolute difference between images
        cv::imshow("Difference Image", diff);  // Display the difference image
        cv::waitKey(0);
    }

    // Function to compare two images
    void compareImages() {
        if (img1.empty() || img2.empty()) {
            std::cerr << "Error: Images not loaded." << std::endl;
            return;
        }

        double mse = calculateMSE();
        double psnr = calculatePSNR();

        std::cout << "MSE: " << mse << std::endl;
        std::cout << "PSNR: " << psnr << " dB" << std::endl;

        // Display the difference image
        displayDifferenceImage();
    }
};
