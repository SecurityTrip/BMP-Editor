#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

class BMPImage {
    private:
        BITMAPFILEHEADER fileHeader;
        BITMAPINFOHEADER infoHeader;
        std::vector<unsigned char> pixelData;
        int width, height, rowSize;
    
        void drawLine(int x1, int y1, int x2, int y2, unsigned char color){
            int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
            int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
            int err = dx + dy, e2;

            while (true) {
                int index = y1 * rowSize + x1 * (infoHeader.biBitCount / 8);
                pixelData[index] = pixelData[index + 1] = pixelData[index + 2] = color;
                if (x1 == x2 && y1 == y2) break;
                e2 = 2 * err;
                if (e2 >= dy) { err += dy; x1 += sx; }
                if (e2 <= dx) { err += dx; y1 += sy; }
            }
        }
    
    public:
        bool load(const std::string& filename){
            std::ifstream file(filename, std::ios::binary);
            if (!file) {
                std::cerr << "Error: Cannot open file " << filename << std::endl;
                return false;
            }

            file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
            file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

            if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
                std::cerr << "Error: Unsupported BMP format. Only 24-bit and 32-bit BMPs are supported." << std::endl;
                return false;
            }

            width = infoHeader.biWidth;
            height = std::abs(infoHeader.biHeight);
            rowSize = ((infoHeader.biBitCount / 8) * width + 3) & ~3;
            pixelData.resize(rowSize * height);
            file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

            file.close();
            return true;
        }
        void display(){
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int index = y * rowSize + x * (infoHeader.biBitCount / 8);
                    unsigned char r = pixelData[index + 2];
                    unsigned char g = pixelData[index + 1];
                    unsigned char b = pixelData[index];
                    if (r == 0 && g == 0 && b == 0)
                        std::cout << "#";
                    else
                        std::cout << " ";
                }
                std::cout << std::endl;
            }
        }
        void drawX(){
            drawLine(0, 0, width - 1, height - 1, 0);
            drawLine(width - 1, 0, 0, height - 1, 0);
        }
        bool save(const std::string& filename){
            std::ofstream file(filename, std::ios::binary);
            if (!file) {
                std::cerr << "Error: Cannot save file " << filename << std::endl;
                return false;
            }
            file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
            file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
            file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());
            file.close();
            return true;
        }
};