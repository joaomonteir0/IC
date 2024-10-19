#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>  // For popen and pclose

class ImageProcessor {
private:
    cv::Mat img;  // Variable to store the image

public:
    // Function to load the image
    bool loadImage(const std::string &imagePath) {
        img = cv::imread(imagePath);  // Load the image using OpenCV
        if (img.empty()) {            // Check if the image was loaded correctly
            std::cerr << "Error: Image is empty." << std::endl;
            return false;
        }
        return true;
    }

    // Function to calculate the histogram of a grayscale image
    std::vector<int> calculateHistogram() {
        if (img.empty()) {
            std::cerr << "Image not loaded." << std::endl;
            return std::vector<int>(256, 0);  // Return empty histogram
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
        // Open a pipe to Gnuplot
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

    // Function to generate the histogram with Gnuplot
    void plotHistogramImage() {
        std::vector<int> histogram = calculateHistogram();
        plotHistogramWithGnuplot(histogram);
    }

    // Function to apply Gaussian filter and display original and filtered image
    void applyGaussianFilter(int kernelSize) {
        if (img.empty()) {
            std::cerr << "Error: No image loaded." << std::endl;
            return;
        }

        // Ensure kernel size is odd and >= 3
        if (kernelSize % 2 == 0) {
            kernelSize += 1;  // Convert even to odd
        }
        if (kernelSize < 3) {
            kernelSize = 3;  // Minimum kernel size
        }

        cv::Mat blurredImage;
        cv::GaussianBlur(img, blurredImage, cv::Size(kernelSize, kernelSize), 0);

        // Display the original image
        cv::imshow("Original Image", img);

        // Display the blurred image
        cv::imshow("Gaussian Blurred Image", blurredImage);
        cv::waitKey(0);  // Wait for the user to close the window

        // Save the blurred image
        std::string outputFile = "blurred_image.png";
        cv::imwrite(outputFile, blurredImage);
        std::cout << "Blurred image saved: " << outputFile << std::endl;
    }
};
