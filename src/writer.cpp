#include "writer.h"
#include "ioport.h"

#define BACKSPACE_LIMIT 2

void Writer::fixPos() {
    if (c >= WIDTH) {
        c = 0;
        r++;
    }
    if (r >= HEIGHT) {
        r--;
        scrollScreen();
    }
    moveCursor(r, c);
}

void Writer::nextChar() {
    c++;
    fixPos();
}

void Writer::moveCursor(int r, int c) {
    unsigned short offset = r * WIDTH + c;
    port_byte_out(0x3d4, 0x0f);
    port_byte_out(0x3d5, (char)(offset & 0xff));
    port_byte_out(0x3d4, 0x0e);
    port_byte_out(0x3d5, (char)((offset >> 8) & 0xff));
}

void Writer::backspace() {
    if (c > BACKSPACE_LIMIT) {
        c--;
        *(offset(r, c)) = ' ';
        *(offset(r, c) + 1) = color;
        moveCursor(r, c);
    }
}

Writer::Writer() {
    r = 0;
    c = 0;
    color = WHITE_ON_BLACK;
}

Writer& Writer::operator<<(int i) {
    printInt(i);
    return *this;
}

Writer& Writer::operator<<(char c) {
    printChar(c);
    return *this;
}

Writer& Writer::operator<<(char string[]) {
    print(string);
    return *this;
}

Writer& Writer::operator<<(const char* string) {
    print(string);
    return *this;
}

void Writer::setColor(char color) {
    this->color = color;
}

void Writer::clearScreen() {
    for (int i = 0; i < 2 * WIDTH * HEIGHT; i += 2) {
        *(short*)(i + VIDEO_BUFFER) = 0;
    }
    r = 0;
    c = 0;
}

void Writer::setPos(int r, int c) {
    this->r = r;
    this->c = c;
}
//TODO: optimize
void Writer::scrollScreen() {
    for (int r = 1; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            *(short*)offset(r - 1, c) = *(short*)offset(r, c);
        }
    }
    for (int c = 0; c < WIDTH; c++) {
        *(short*)offset(HEIGHT - 1, c) = 0;
    }
}

void Writer::nextLine() {
    c = 0;
    r++;
    fixPos();
}

void Writer::print(char string[]) {
    for (int i = 0; string[i] != 0; i++) {
        printChar(string[i]);
    }
}

void Writer::print(const char* string) {
    for (int i = 0; string[i] != 0; i++) {
        printChar(string[i]);
    }
}

void Writer::printInt(int n) {
    printIntBase(n, 10, 0);
}

void Writer::printChar(char character) {
    if (character == '\n') {
        nextLine();
    } else if (character == '\b') {
        backspace();
    } else {
        *(offset(r, c)) = character;
        *(offset(r, c) + 1) = color;
        nextChar();
    }
}

void Writer::printIntBase(int n, int base, int fill) {
    if (n == 0) {
        printChar('0');
        for (int i = 0; i < fill - 1; i++) {
            printChar('0');
        }
        return;
    }
    char temp[32];
    if (n < 0) {
        n = -n;  //doesn't work with -2147483648, but who cares tbh
        printChar('-');
    }
    for (int i = 0; i < 32; i++) {
        temp[i] = n % base;
        n /= base;
    }
    bool print = false;
    for (int i = 31; i >= 0; i--) {
        if (temp[i] != 0 || print || fill > i) {
            print = true;
            if (temp[i] < 10) {
                printChar('0' + temp[i]);
            } else {
                printChar('a' - 10 + temp[i]);
            }
        }
    }
}
