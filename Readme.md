# Mandelbrot

## Usage
You have to pass the following command line arguments:
1. width of image
2. height of image
3. minX of the viewPort
4. minY of the viewPort
5. maxX of the viewPort
6. maxY of the viewPort
7. maxIterations

**Example:**
ParallelMandelbrot.exe 1024 1024 -2 -1 1 1 300


## Implementation

There are 2 methods **mandelbrot** and **parallelMandelbrot**, which are defined in **mandelbrot.h**.

### ParallelMandelbrot
I decided to use a static scheduler due to the fact that the time for each iteration will not differ a lot 

In main.cpp we measure the performance of the 2 methods using std::chrono. 