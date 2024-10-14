#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath> // Include cmath for std::round()

class ImageProcessor {
private:
    cv::Mat img;  // Variable to store the image

public:
    // Function to load the image
    bool loadImage(const std::string &imagePath) {
        img = cv::imread(imagePath);  // Load the image using OpenCV
        if (img.empty()) {            // Check if the image was loaded correctly
            std::cerr << "Erro: Imagem vazia." << std::endl;
            return false;
        }
        return true;
    }

    // Function to display the image
    void displayImage(const std::string &windowName) {
        cv::imshow(windowName, img);  // Display the image in the window
        cv::waitKey(0);               // Wait for 1 millisecond
    }

    // function to split the image channels
    void displayChannels() {
        if (img.empty()) {
            std::cerr << "Load an image" << std::endl;
            return;
        }

        std::vector<cv::Mat> channels;
        cv::split(img, channels);  // Split the image into channels

        cv::imshow("Blue Channel", channels[0]);  // Blue channel
        cv::imshow("Green Channel", channels[1]); // Green channel
        cv::imshow("Red Channel", channels[2]);   // Red channel

        cv::Mat grayscale;
        cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);  // Convert to grayscale

        cv::imshow("Grayscale", grayscale); 
        cv::waitKey(0); 
    }

    void displayHistogram() {
        if (img.empty()) {
            std::cerr << "Imagem não carregada." << std::endl;
            return;
        }

        // Convert to grayscale if not already
        cv::Mat grayscale;
        if (img.channels() == 3) {
            cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);
        } else {
            grayscale = img.clone();
        }

        // Calculate the histogram
        int histSize = 256; // Number of bins for pixel intensities (0 to 255)
        float range[] = {0, 256}; // Range of intensity values
        const float* histRange = {range};
        cv::Mat hist;

        cv::calcHist(&grayscale, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

        // Normalize the histogram to fit within the display
        int hist_w = 512;
        int hist_h = 400;
        int bin_w = std::round((double)hist_w / histSize);
        cv::Mat histImage(hist_h + 100, hist_w + 100, CV_8UC3, cv::Scalar(255, 255, 255));  // +100 for more space

        // Normalize the result to [0, histImage.rows - 50] (leave space for labels)
        cv::normalize(hist, hist, 0, hist_h, cv::NORM_MINMAX);

        // Draw the intensity histogram
        for (int i = 1; i < histSize; i++) {
            cv::line(histImage, 
                     cv::Point(50 + bin_w * (i - 1), hist_h - std::round(hist.at<float>(i - 1))),  // Leave margin for Y-axis
                     cv::Point(50 + bin_w * (i), hist_h - std::round(hist.at<float>(i))),         // Leave margin for Y-axis
                     cv::Scalar(0, 0, 0), 2, 8, 0);
        }

        // Adding X and Y axis labels
        cv::putText(histImage, "Intensity", cv::Point(hist_w / 2, hist_h + 70), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0), 2);
        cv::putText(histImage, "Frequency", cv::Point(20, hist_h / 2), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0), 2);

        // Adding tick marks and values for X-axis (Intensity)
        for (int i = 0; i <= 255; i += 50) {
            cv::line(histImage, cv::Point(50 + bin_w * i, hist_h), cv::Point(50 + bin_w * i, hist_h + 10), cv::Scalar(0), 2); // Tick mark
            cv::putText(histImage, std::to_string(i), cv::Point(45 + bin_w * i, hist_h + 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0), 1);
        }

        // Adding tick marks and values for Y-axis (Frequency)
        for (int i = 0; i <= 5; i++) {
            int val = hist_h - (i * (hist_h / 5));
            cv::line(histImage, cv::Point(45, val), cv::Point(50, val), cv::Scalar(0), 2); // Tick mark
            cv::putText(histImage, std::to_string(i * 20), cv::Point(10, val + 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0), 1);
        }

        // Display the histogram with labels
        cv::imshow("Grayscale Histogram with Labels", histImage);
        cv::waitKey(0);
    }
};
