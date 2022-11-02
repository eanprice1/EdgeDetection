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


void ReadImageFile(int** image, char* fileName, size_t rows, size_t cols) {
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

int** CreateTwoDimArray(size_t rows, size_t cols) {
    int** twoDimArray = new int* [rows];
    for (size_t i = 0; i < rows; i++) {
        twoDimArray[i] = new int[cols];
        for (size_t j = 0; j < cols; j++) {
            twoDimArray[i][j] = 0;
        }
    }
    return twoDimArray;
}

void CalculateGradient(int** horizontalImage, int** verticalImage, int** gradientImage, int minThreshold, int maxThreshold, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            int xSquared = horizontalImage[i][j] * horizontalImage[i][j];
            int ySquared = verticalImage[i][j] * verticalImage[i][j];
            int sum = xSquared + ySquared;
            int result = (int) ceil(sqrt(sum));
            
            if (result < minThreshold || result > maxThreshold) {
                gradientImage[i][j] = 200;
            }
            else {
                gradientImage[i][j] = 0;
            }
        }
    }

}

void ProcessImage(char* horizontalFilename, char* verticalFilename, char* outputGradientFilename, size_t rows, size_t cols) {
    int** horizontalImage = CreateTwoDimArray(rows, cols);
    int** verticalImage = CreateTwoDimArray(rows, cols);
    int** gradientImage = CreateTwoDimArray(rows, cols);
    ReadImageFile(horizontalImage, horizontalFilename, rows, cols);
    ReadImageFile(verticalImage, verticalFilename, rows, cols);
    CalculateGradient(horizontalImage, verticalImage, gradientImage, 175, 210, rows, cols);
    WriteImageFile(gradientImage, outputGradientFilename, rows, cols);
}

int main()
{
    ProcessImage(horizontal3x3ImageName, vertical3x3ImageName, gradient3x3ImageName, imageRows, imageCols);
    ProcessImage(horizontal5x5ImageName, vertical5x5ImageName, gradient5x5ImageName, imageRows, imageCols);
    return 0;
}

