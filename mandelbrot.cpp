//
// Created by vik on 15/03/2021.
//
#include <string>
#include <iostream>
#include "mandelbrot.h"
#include "tga.h"
#include <omp.h>

double calculateScalingFactor(int viewPortMax, int viewPortMin, int windowMax, int windowMin) {
    return (viewPortMax - viewPortMin) / (double) (windowMax - windowMin);
}

double normalize(int p, int viewPortMin, int windowMin, double scalingFactor) {
    return viewPortMin + ((p - windowMin) * scalingFactor);
}

std::tuple<double, double> normalizeToViewRectangle(int px, int py, Rectangle viewPort, Rectangle window) {
    auto sX = calculateScalingFactor(viewPort.maxX, viewPort.minX, window.maxX, window.minX);
    auto sY = calculateScalingFactor(viewPort.maxY, viewPort.minY, window.maxY, window.minY);
    auto cx = normalize(px, viewPort.minX, window.minX, sX);
    auto cy = normalize(py, viewPort.minY, window.minY, sY);
    return std::make_tuple(cx, cy);
}

int calcColor(int px, int py, Rectangle viewPort, Rectangle window) {
    const auto normalizedCoordinates = normalizeToViewRectangle(px, py, viewPort, window);
    auto cx = std::get<0>(normalizedCoordinates);
    auto cy = std::get<1>(normalizedCoordinates);
    auto zx = cx;
    auto zy = cy;
    for (auto n = 0; n < 255; n++) {
        double x = (zx * zx - zy * zy) + cx;
        double y = (zy * zx + zx * zy) + cy;
        if ((x * x + y * y) > 4) {
            // diverge paint pixel lighter (depending on n)
            return 255 - n;
        }
        zx = x;
        zy = y;
    }
    return 0;
}

tga::TGAImage InitializeImage(int width, int height) {
    tga::TGAImage image;
    image.width = width;
    image.height = height;
    image.type = 0;
    image.bpp = 24;
    image.imageData.assign(image.width * image.height * 3, 0);
    return image;
}

void mandelbrot(Rectangle viewPort, Rectangle window, const std::string &filename) {
    tga::TGAImage image = InitializeImage(window.maxX, window.maxY);

    for (auto py = 0; py < image.height; py++) {
        for (auto px = 0; px < image.width; px++) {
            double color = calcColor(px, py, viewPort, window);
            int pixelIndex = (py * window.maxX + px) * 3;
            image.imageData.at(pixelIndex + 0) = color;
            image.imageData.at(pixelIndex + 1) = color;
            image.imageData.at(pixelIndex + 2) = color;
        }
    }
    tga::saveTGA(image, filename.c_str());
}

void parallelMandelbrot(Rectangle viewPort, Rectangle window, const std::string &filename, int numThreads) {
    tga::TGAImage image = InitializeImage(window.maxX, window.maxY);

    omp_set_num_threads(numThreads);
    int chunkSize = image.width * image.height * 3;
    int chunkH = window.maxY / numThreads;
    std::vector<unsigned char> imageData;

#pragma omp parallel firstprivate(imageData)
    int id = omp_get_thread_num();
    int localH = id + chunkH;
    #pragma omp parallel for collapse(2) firstprivate(id)
        for (auto py = 0; py < localH; py++) {
            for (auto px = 0; px < image.width; px++) {
                double color = calcColor(px, py, viewPort, window);
                int pixelIndex = (py * window.maxX + px) * 3;
                imageData.at(pixelIndex + 0) = color;
                imageData.at(pixelIndex + 1) = color;
                imageData.at(pixelIndex + 2) = color;
            }
        }
        int startIndex = id * chunkSize;
        #pragma omp critical
            std::copy(imageData.begin(), imageData.end(), image.imageData.begin() + startIndex);

    tga::saveTGA(image, filename.c_str());
}





