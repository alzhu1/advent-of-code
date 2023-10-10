#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    if (file.is_open()) {
        file >> line;
        file.close();
    }
    int layerSize = 25 * 6;

    // Part 1
    int numLayers = line.length() / layerSize;
    int bestLayer = 0;
    int bestCount = layerSize;
    int bestProduct = 0;
    for (int layer = 0; layer < numLayers; layer++) {
        int zeroCount = 0;
        int oneCount = 0;
        int twoCount = 0;
        for (int i = 0; i < (25 * 6); i++) {
            char c = line[(layer * layerSize) + i];
            
            if (c == '0') {
                zeroCount++;
            }
            if (c == '1') {
                oneCount++;
            }
            if (c == '2') {
                twoCount++;
            }
        }

        if (zeroCount < bestCount) {
            bestCount = zeroCount;
            bestLayer = layer;
            bestProduct = oneCount * twoCount;
        }
    }

    std::cout << bestProduct << std::endl;

    // Part 2
    std::ofstream outFile;
    outFile.open ("output.txt");
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 25; x++) {
            int index = (y * 25) + x;
            char pixelValue = '2';

            for (int layer = 0; layer < numLayers; layer++) {
                char pix = line[(layer * layerSize) + index];
                if (pix != '2') {
                    pixelValue = pix;
                    break;
                }
            }

            outFile << (pixelValue == '1' ? '#' : '.');
        }
        outFile << std::endl;
    }

    outFile.close();
}