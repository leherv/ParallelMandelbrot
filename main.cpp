#include <iostream>
#include "mandelbrot.h"
#include <string>
#include <fstream>

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

    /*
    long sumMs = 0;
    int numberExecutions = 100;
    for(int i = 0; i < numberExecutions; i++) {
        // sumMs += mandelbrot(viewPort, window, iterations, "mandelbrot.tga");
        sumMs += parallelMandelbrot(viewPort, window, iterations, "mandelbrot.tga", 20);
    }
    long avgMs = sumMs / numberExecutions;
    */

    std::ofstream csv;
    csv.open ("performance.csv");
    csv << "threads, execution time (ms), speedup" << std::endl;
    auto speedSingleThread = avgTimeInMs(10, viewPort, window, iterations, 1);
    csv << 1 << "," << speedSingleThread << "," << 0 << std::endl;
    for(int i = 2; i <= 100; i++) {
        auto avgTimeMs = avgTimeInMs(10, viewPort, window, iterations, i);
        csv << i << "," << avgTimeMs << "," << speedSingleThread/avgTimeMs << std::endl;
    }
    csv.close();

    return 0;
}




