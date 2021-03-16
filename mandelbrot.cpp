//
// Created by vik on 15/03/2021.
//
#include <string>
#include <iostream>
#include "mandelbrot.h"
#include "tga.h"
#include <omp.h>
#include <numeric>

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

int calcColor(int px, int py, Rectangle viewPort, Rectangle window, int maxIterations) {
    const auto normalizedCoordinates = normalizeToViewRectangle(px, py, viewPort, window);
    auto cx = std::get<0>(normalizedCoordinates);
    auto cy = std::get<1>(normalizedCoordinates);
    auto zx = cx;
    auto zy = cy;
    for (auto n = 0; n < maxIterations; n++) {
        double x = (zx * zx - zy * zy) + cx;
        double y = (zy * zx + zx * zy) + cy;
        if ((x * x + y * y) > 4) {
            // diverge paint pixel lighter (depending on n)
            return 255 * (n / maxIterations);
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

void mandelbrot(Rectangle viewPort, Rectangle window, int maxIterations, const std::string &filename) {
    tga::TGAImage image = InitializeImage(window.maxX, window.maxY);

    for (auto py = 0; py < image.height; py++) {
        for (auto px = 0; px < image.width; px++) {
            double color = calcColor(px, py, viewPort, window, maxIterations);
            int pixelIndex = (py * window.maxX + px) * 3;
            image.imageData.at(pixelIndex + 0) = color;
            image.imageData.at(pixelIndex + 1) = color;
            image.imageData.at(pixelIndex + 2) = color;
        }
    }
    tga::saveTGA(image, filename.c_str());
}

std::vector<int> getChunks(int whole, int numberParts) {
    auto chunkSize = (float)whole/(float)numberParts;
    int chunkSizeNoFraction = floor(chunkSize);
    std::vector<int> chunks(numberParts, chunkSizeNoFraction);
    // fraction
    if((float)chunkSizeNoFraction != chunkSize) {
        // make last chunk slightly bigger
        chunks.at(numberParts - 1) = whole - ((numberParts-1) * chunkSizeNoFraction);
    }
    return chunks;
}

int getX(int pixelIndex, int width) {
    return pixelIndex % width;
}

int getY(int pixelIndex, int width) {
    return pixelIndex / width;
}

void parallelMandelbrot(Rectangle viewPort, Rectangle window, int maxIterations, const std::string &filename, int numThreads) {
    tga::TGAImage image = InitializeImage(window.maxX, window.maxY);
    std::vector<std::vector<unsigned char>> imageByteParts(numThreads);

    omp_set_num_threads(numThreads);
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int pixels = window.maxY * window.maxX;
        auto pixelChunks = getChunks(pixels, threads);

        int id = omp_get_thread_num();
        std::vector<unsigned char> byteParts(pixelChunks.at(id) * 3, 0);
        unsigned int startIndex = std::accumulate(pixelChunks.begin(), pixelChunks.begin() + id, 0);
        int width = window.maxX;

        #pragma omp parallel for collapse(2) schedule(static)
        for (auto localPixelIndex = 0; localPixelIndex < pixelChunks.at(id); localPixelIndex++) {
            int pixelIndexWindow = startIndex + localPixelIndex;
            int px = getX(pixelIndexWindow, width);
            int py = getY(pixelIndexWindow, width);
            double color = calcColor(px, py, viewPort, window, maxIterations);
            int localByteIndex = localPixelIndex * 3;
            byteParts.at(localByteIndex + 0) = color;
            byteParts.at(localByteIndex + 1) = color;
            byteParts.at(localByteIndex + 2) = color;
        }
        imageByteParts.at(id) = byteParts;
    }
    unsigned int startIndex = 0;
    for(auto imageBytePart : imageByteParts) {
        std::copy(imageBytePart.begin(), imageBytePart.end(), image.imageData.begin() + startIndex);
        startIndex += imageBytePart.size();
    }
    tga::saveTGA(image, filename.c_str());
}





