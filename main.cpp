#include <iostream>
#include "BMPImage.h"


int main() {
    std::string inputFile, outputFile;
    std::cout << ">> Enter input BMP file name: ";
    std::cin >> inputFile;

    BMPImage image;
    if (!image.load(inputFile)) return 1;

    image.display();
    image.drawX();
    std::cout << "\nModified Image with X:" << std::endl;
    image.display();

    std::cout << ">> Enter output BMP file name: ";
    std::cin >> outputFile;
    if (!image.save(outputFile)) return 1;

    std::cout << "Image saved successfully!" << std::endl;
    return 0;
}