#include "mandelbrot.h"

inline float map(float x, float in_max, float out_min, float out_max) {
    return x * (out_max - out_min) / (in_max) + out_min;
}

inline int clamp(int x, int min, int max) {
    if (x > max) {
        x = max;
    } else if (x < min) {
        x = min;
    }
    return x;
}

float mandelbrot(int xIn, int yIn, int width, int height) {
    float x0 = map(xIn, width, -2., .47);
    float y0 = map(yIn, height, -1.12, 1.12);
    float x = 0;
    float y = 0;
    int maxI = 30;
    int i;
    for (i = 0; i < maxI && x * x + y * y <= 4; i++) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
    }
    return (float)i / maxI;
}

char mandelbrotColor(float f) {
    int n = clamp(f * 5, 0, 5);
    return (5 - n << 4) + 0xf;
}

void runMandelbrot(Writer& writer, int width, int height) {
    writer.clearScreen();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            writer.setColor(mandelbrotColor(mandelbrot(x, y, width, height)));
            writer << ' ';
        }
    }
    writer.setColor(WHITE_ON_BLACK);
}