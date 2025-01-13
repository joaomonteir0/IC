#ifndef PREDICTORS_H
#define PREDICTORS_H

#include <algorithm>
#include <iostream> // For debugging output

class Predictors {
public:
    enum Standards {
        JPEG_PL,  // Pixel Left
        JPEG_PA,  // Pixel Above
        JPEG_PAL, // Pixel Above Left
        JPEG_ABC, // A + B - C (three neighbors)
        JPEG_MBC, // Median Adjusted by C
        JPEG_LS   // JPEG-LS predictor
    };

    int predictors(Standards standard, int a, int b, int c) {
        int prediction = 0;
        switch (standard) {
        case JPEG_PL:
            prediction = a;
            break;
        case JPEG_PA:
            prediction = b;
            break;
        case JPEG_PAL:
            prediction = c;
            break;
        case JPEG_ABC:
            prediction = a + b - c;
            break;
        case JPEG_MBC:
            prediction = a + (b - c) / 2;
            break;
        case JPEG_LS: {
            int mini = std::min(a, b);
            int maxi = std::max(a, b);
            if (c >= maxi)
                prediction = mini;
            else if (c <= mini)
                prediction = maxi;
            else
                prediction = a + b - c;
            break;
        }
        default:
            prediction = 0; // invalid predictors

        }

        // logs for debugging
        /*std::cout << "Predictor: " << standard
                  << ", A: " << a << ", B: " << b << ", C: " << c
                  << ", Prediction: " << prediction << "\n";
        */              
        return prediction;
    }
};

#endif // PREDICTORS_H
