#include <iostream>
#include <fstream>
#include <math.h>
#pragma warning(disable : 4996)

const size_t imageRows = 500;
const size_t imageCols = 750;
char horizontal3x3ImageName[50] = "./ImageHorizontal_3x3.raw";
char vertical3x3ImageName[50] = "./ImageVertical_3x3.raw";
char horizontal5x5ImageName[50] = "./ImageHorizontal_5x5.raw";
char vertical5x5ImageName[50] = "./ImageVertical_5x5.raw";
char gradient3x3ImageName[50] = "./Gradient_3x3.raw";
char gradient5x5ImageName[50] = "./Gradient_5x5.raw";


void ReadIntImageFile(int** image, char* fileName, size_t rows, size_t cols) {
    FILE* inputFile = fopen(fileName, "rb");

    if (inputFile) {
        std::cout << "Reading " << fileName << " into image array" << std::endl;
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                image[i][j] = fgetc(inputFile);
            }
        }
        fclose(inputFile);
        std::cout << "Completed read " << std::endl;
    }
}

void ReadFloatImageFile(float** image, char* fileName, size_t rows, size_t cols) {
    FILE* inputFile = fopen(fileName, "rb");

    if (inputFile) {
        std::cout << "Reading " << fileName << " into image array" << std::endl;
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                image[i][j] = (float) fgetc(inputFile);
            }
        }
        fclose(inputFile);
        std::cout << "Completed read " << std::endl;
    }
}

void WriteImageFile(int** image, char* fileName, size_t rows, size_t cols) {
    FILE* outputFile = fopen(fileName, "wb");

    std::cout << "Writing to " << fileName << " from image array" << std::endl;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            fputc(image[i][j], outputFile);
        }
    }
    fclose(outputFile);
    std::cout << "Completed Write" << std::endl;
}

int** CreateIntTwoDimArray(size_t rows, size_t cols) {
    int** twoDimArray = new int* [rows];
    for (size_t i = 0; i < rows; i++) {
        twoDimArray[i] = new int[cols];
        for (size_t j = 0; j < cols; j++) {
            twoDimArray[i][j] = 0;
        }
    }
    return twoDimArray;
}

float** CreateFloatTwoDimArray(size_t rows, size_t cols) {
    float** twoDimArray = new float* [rows];
    for (size_t i = 0; i < rows; i++) {
        twoDimArray[i] = new float[cols];
        for (size_t j = 0; j < cols; j++) {
            twoDimArray[i][j] = 0;
        }
    }
    return twoDimArray;
}

void Calculate3x3Gradient(float** horizontalImage, float** verticalImage, int** gradientImage, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            float xSquared = horizontalImage[i][j] * horizontalImage[i][j];
            float ySquared = verticalImage[i][j] * verticalImage[i][j];
            float sum = xSquared + ySquared;
            int result = (int)ceil(sqrt(sum));
            gradientImage[i][j] = result / 2;
        }
    }
}

void Calculate5x5Gradient(float** horizontalImage, float** verticalImage, int** gradientImage, size_t rows, size_t cols) {
    int max = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            float xSquared = horizontalImage[i][j] * horizontalImage[i][j];
            float ySquared = verticalImage[i][j] * verticalImage[i][j];
            float sum = xSquared + ySquared;
            int result = (int)ceil(sqrt(sqrt(sum)));
            if (result > max) {
                max = result;
            }
            gradientImage[i][j] = result * 3;
        }
    }
    std::cout << "max: " << max << std::endl;
}

void ReverseNormalization(float** image, int min, int max, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            float reversedValue = image[i][j] / 255;
            reversedValue = reversedValue * (max - min) + min;
            image[i][j] = reversedValue;
        }
    }
}

void ProcessImage(char* horizontalFilename, char* verticalFilename, char* outputGradientFilename, int hMin, int hMax, int vMin, int vMax, size_t rows, size_t cols, bool is5x5=false) {
    float** horizontalImage = CreateFloatTwoDimArray(rows, cols);
    float** verticalImage = CreateFloatTwoDimArray(rows, cols);
    int** gradientImage = CreateIntTwoDimArray(rows, cols);
    ReadFloatImageFile(horizontalImage, horizontalFilename, rows, cols);
    ReadFloatImageFile(verticalImage, verticalFilename, rows, cols);
    ReverseNormalization(horizontalImage, hMin, hMax, imageRows, imageCols);
    ReverseNormalization(verticalImage, vMin, vMax, imageRows, imageCols);
    if (is5x5) {
        Calculate5x5Gradient(horizontalImage, verticalImage, gradientImage, imageRows, imageCols);

    }
    else {
        Calculate3x3Gradient(horizontalImage, verticalImage, gradientImage, imageRows, imageCols);
    }
    WriteImageFile(gradientImage, outputGradientFilename, rows, cols);
}

int main()
{
    ProcessImage(horizontal3x3ImageName, vertical3x3ImageName, gradient3x3ImageName, -908, 778, -831, 851, imageRows, imageCols);
    ProcessImage(horizontal5x5ImageName, vertical5x5ImageName, gradient5x5ImageName, -5436, 5294, -5984, 5712, imageRows, imageCols, true);
    return 0;
}

