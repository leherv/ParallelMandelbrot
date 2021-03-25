//
// Created by vik on 15/03/2021.
//

#ifndef PARALLELMANDELBROT_MANDELBROT_H
#define PARALLELMANDELBROT_MANDELBROT_H

#include <tuple>


typedef struct {
    int minX;
    int maxX;
    int minY;
    int maxY;
} Rectangle;

double mandelbrot(Rectangle viewPort, Rectangle window, int maxIterations, const std::string& filename);
double parallelMandelbrot(Rectangle viewPort, Rectangle window, int maxIterations, const std::string &filename, int numThreads);
double avgTimeInMs(int numberExecutions, Rectangle viewPort, Rectangle window, int iterations, int numThreads);

#endif //PARALLELMANDELBROT_MANDELBROT_H
