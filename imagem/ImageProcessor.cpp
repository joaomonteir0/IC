#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>  // For system()

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

    // Function to calculate the histogram of a grayscale image
    std::vector<int> calculateHistogram() {
        if (img.empty()) {
            std::cerr << "Imagem não carregada." << std::endl;
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

    // Function to save the histogram data to a file for Gnuplot
    void saveHistogramToFile(const std::vector<int>& histogram, const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "Erro ao abrir o arquivo para salvar o histograma." << std::endl;
            return;
        }

        // Write the histogram data to the file
        for (int i = 0; i < 256; ++i) {
            outFile << i << " " << histogram[i] << "\n";  // Intensity value and its frequency
        }

        outFile.close();
    }

    void generateGnuplotScript(const std::string& dataFile, const std::string& scriptFile, const std::string& outputFile) {
        std::ofstream script(scriptFile);
        if (!script.is_open()) {
            std::cerr << "Erro ao criar o arquivo de script do Gnuplot." << std::endl;
            return;
        }

        script << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
        script << "set output '" << outputFile << "'\n";
        script << "set title 'Grayscale Histogram'\n";
        script << "set xlabel 'Intensity'\n";
        script << "set ylabel 'Frequency'\n";
        script << "set boxwidth 0.9 relative\n";  // Ajusta a largura da barra
        script << "set style fill solid 1.0\n";   // Barras sólidas
        script << "set yrange [0:*]\n";           // Ajusta automaticamente o eixo Y
        // Para definir manualmente o intervalo do eixo Y, substitua a linha acima por:
        // script << "set yrange [0:5000]\n";     // Limite manual do eixo Y
        script << "plot '" << dataFile << "' using 1:2 with boxes lc rgb 'blue' notitle\n";

        script.close();
    }

    // Function to run Gnuplot to generate the histogram
    void plotHistogramWithGnuplot() {
        // Step 1: Calculate the histogram
        std::vector<int> histogram = calculateHistogram();

        // Step 2: Save the histogram data to a file
        std::string dataFile = "histogram_data.txt";
        saveHistogramToFile(histogram, dataFile);

        // Step 3: Generate the Gnuplot script
        std::string scriptFile = "plot_histogram.gnuplot";
        std::string outputFile = "histogram.png";
        generateGnuplotScript(dataFile, scriptFile, outputFile);

        // Step 4: Call Gnuplot to execute the script
        std::string command = "gnuplot " + scriptFile;
        system(command.c_str());

        std::cout << "Histograma gerado: " << outputFile << std::endl;

        // Optional: Clean up temporary files
        // remove(dataFile.c_str());
        // remove(scriptFile.c_str());
    }
};

