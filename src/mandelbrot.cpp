#include "mandelbrot.h"

#define map(x, in_max, out_min, out_max) (x) * (out_max - out_min) / (in_max) + out_min;

float mandelbrot(int xIn, int yIn, int width, int height) {
    float x0 = map((float)xIn, width, -2., .47);
    float y0 = map((float)yIn, height, -1.12, 1.12);
    float x = 0;
    float y = 0;
    int maxI = 30;
    int i;
    for (i = 0; i < maxI && x * x + y * y <= 4; i++) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
    }
    return (float) i / maxI;
}

char mandelbrotColor(float f) {
    int n = (int)(f * 5);
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