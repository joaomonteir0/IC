#ifndef PREDICTORS_H
#define PREDICTORS_H

#include <algorithm>

class Predictors {
public:
    // Enum to represent the different prediction standards
    enum Standards {
        JPEG_PL,  // Pixel left
        JPEG_PA,  // Pixel above
        JPEG_PAL, // Pixel above left
        JPEG_ABC, // A + B - C (three neighbors)
        JPEG_MBC, // Median between A and B, adjusted by C
        JPEG_LS   // JPEG-LS predictor
    };

    // Function to apply the selected predictor
    int predictors(Standards standard, int a, int b, int c) {
        switch (standard) {
        case JPEG_PL:
            return a; // Pixel left
        case JPEG_PA:
            return b; // Pixel above
        case JPEG_PAL:
            return c; // Pixel above left
        case JPEG_ABC:
            return a + b - c; // A + B - C
        case JPEG_MBC:
            return a + (b - c) / 2; // Median adjusted by C
        case JPEG_LS: {
            int pred = a + b - c;
            int mini = std::min(a, b);
            int maxi = std::max(a, b);

            if (c >= maxi) {
                pred = mini;
            } else if (c <= mini) {
                pred = maxi;
            }

            return pred;
        }
        default:
            return 0; // Default to 0 for unsupported standards
        }
    }
};

#endif // PREDICTORS_H
