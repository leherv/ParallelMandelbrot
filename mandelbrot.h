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

void mandelbrot(Rectangle viewPort, Rectangle window, const std::string& filename);
void parallelMandelbrot(Rectangle viewPort, Rectangle window, const std::string &filename, int numThreads);


#endif //PARALLELMANDELBROT_MANDELBROT_H
