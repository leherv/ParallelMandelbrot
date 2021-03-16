#include <iostream>
#include "mandelbrot.h"
#include <string>
#include <chrono>

int main(int argc, char *argv[]) {

    const int numberArguments = 8;
    if(argc != numberArguments) {
        std::cout << "Wrong number of arguments";
        return 1;
    }
    auto width = std::stoi(argv[1]);
    auto height = std::stoi(argv[2]);
    auto minX = std::stoi(argv[3]);
    auto minY = std::stoi(argv[4]);
    auto maxX = std::stoi(argv[5]);
    auto maxY = std::stoi(argv[6]);
    auto iterations = std::stoi(argv[7]);

    Rectangle window;
    window.minX = 0;
    window.maxX = width;
    window.minY = 0;
    window.maxY = height;

    Rectangle viewPort;
    viewPort.minX = minX;
    viewPort.maxX = maxX;
    viewPort.minY = minY;
    viewPort.maxY = maxY;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    mandelbrot(viewPort, window, iterations, "mandelbrot.tga");
    // parallelMandelbrot(viewPort, window, iterations, "mandelbrot.tga", 20);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Execution took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

    return 0;
}
