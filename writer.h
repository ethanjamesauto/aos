#define VIDEO_BUFFER (char*)0xb8000
#define WHITE_ON_BLACK 0xf
#define WIDTH 80
#define HEIGHT 25
#define endl '\n'

#ifndef WRITER_H
#define WRITER_H
class Writer {
    int r;
    int c;
    char color;
    void offset(int r, int c);
    void fixPos();
    void nextChar();
    void moveCursor(int r, int c);
    void backspace();

   public:
    Writer();
    Writer& operator<<(int i);
    Writer& operator<<(char c);
    Writer& operator<<(char string[]);
    Writer& operator<<(const char* string);
    void setColor(char color);
    void clearScreen();
    void setPos(int r, int c);
    void scrollScreen();
    void nextLine();
    void print(char string[]);
    void print(const char* string);
    void printInt(int n);
    void printChar(char character);
    void printIntBase(int n, int base, int fill);
};
#endif