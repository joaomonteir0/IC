#ifndef PREDICTORS_H
#define PREDICTORS_H

#include <algorithm>
#include <iostream> // For debugging output

/*
JPEG_PL (Pixel Left):
- Usa o pixel à esquerda (A) como predição. Bom para padrões horizontais
JPEG_PA (Pixel Above):
- Usa o pixel acima (B) como predição. Bom para padrões verticais
JPEG_PAL (Pixel Above Left):
- Usa o pixel diagonal superior esquerdo (C) como predição. Bom com padrões diagonais.
JPEG_ABC:
- Predict = A + B - C
- Combina A, B e C para calcular a predição. Equilibrado entre horizontais, verticais e diagonais.
JPEG_MBC (Median Adjusted by C):
- Predict = A + (B - C) / 2
- Ajusta os valores de A e B com base no C.
PEG_LS (JPEG-LS Predictor)
Se C >= max(A, B): Predição = min(A, B)  
Se C <= min(A, B): Predição = max(A, B)  
Caso contrário: Predição = A + B - C  
Descrição: Preditor adaptativo que considera as relações entre os vizinhos.
Pontos Fortes: Melhor desempenho geral para padrões complexos ou imagens com ruído.
Fraquezas: Mais pesado computacionalmente.
*/
class Predictors {
public:
    enum Standards {
        JPEG_PL,  // Pixel Left
        JPEG_PA,  // Pixel Above
        JPEG_PAL, // Pixel Above Left
        JPEG_ABC, // A + B - C
        JPEG_MBC, // A + (B - C)/2
        JPEG_LS   // JPEG-LS predictor
    };

    int predictors(Standards standard, int a, int b, int c) {
        switch (standard) {
            case JPEG_PL:  return a; 
            case JPEG_PA:  return b;
            case JPEG_PAL: return c;
            case JPEG_ABC: return a + b - c;
            case JPEG_MBC: return a + (b - c) / 2;
            case JPEG_LS:
                if (c >= std::max(a, b)) return std::min(a, b);
                if (c <= std::min(a, b)) return std::max(a, b);
                return a + b - c;
            default:       return 0;
        }
    }
};

#endif // PREDICTORS_H