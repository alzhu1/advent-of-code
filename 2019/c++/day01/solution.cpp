#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    int sum1 = 0;
    int sum2 = 0;
    if (file.is_open()) {

        while (getline(file, line)) {
            int num = stoi(line);
            num = (num / 3) - 2;
            sum1 += num;

            while (num > 0) {
                sum2 += num;
                num = (num / 3) - 2;
            }
        }
    }
    std::cout << sum1 << std::endl;
    std::cout << sum2 << std::endl;
}