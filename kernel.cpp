#include "kernel.h"

#include "ioport.h"
#include "key_manager.h"
#include "memory_manager.h"
#include "parse_command.h"

#define MAX_COMMAND_SIZE 1000

void start() {
    Writer writer;
    KeyManager k;
    test_routine(writer);
    new_line(writer);
    char* cmd_buffer = (char*)alloc(MAX_COMMAND_SIZE);
    int cmd_buffer_pos = 0;
    while (true) {
        k.update();
        char c = 0;
        while (c = k.getKeyPress()) {  //pog?
            switch (c) {
                case '\n':
                    cmd_buffer[cmd_buffer_pos] = '\0';
                    cmd_buffer_pos = 0;
                    writer << endl;
                    if (!parse_command(cmd_buffer, writer)) {
                        writer << "command \"" << cmd_buffer << "\" not found" << endl;
                    }
                    new_line(writer);
                    break;
                default:
                    if (cmd_buffer_pos < MAX_COMMAND_SIZE) {
                        if (c == '\b') {
                            cmd_buffer_pos--;
                            if (cmd_buffer_pos < 0) {
                                cmd_buffer_pos = 0;
                            }
                        } else {
                            cmd_buffer[cmd_buffer_pos++] = c;
                        }
                        writer << c;
                    }
            }
        }
    }
}

void new_line(Writer& writer) {
    writer << "> ";
}

void print_cmos(Writer& writer) {
    while (true) {
        for (int reg = 0; reg <= 0x3e; reg++) {
            if (reg > 0 && reg % 4 == 0) {
                writer << endl;
            }
            port_byte_out(0x70, reg);
            writer.printIntBase((int)port_byte_in(0x71), 2, 8);
            writer << ' ';
        }
        writer.printIntBase((int)port_byte_in(0x60), 2, 8);
        writer.setPos(0, 0);
    }
}

int fib(int n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

void test_routine(Writer& writer) {
    writer.setColor(0b00011111);
    writer.print("Welcome to AutoOS!\n");
    writer.setColor(WHITE_ON_BLACK);
}

//#define VERBOSE
void check_writeable(Writer& writer) {
    writer << "Writeable regions found: " << endl;
    int prev = 0;
    long long start = 0xb8000 + WIDTH * HEIGHT * 2;
    long long end = 0xffffffff;
    char yes[] = "y";
    char no[] = "n";
    for (long long i = start; i <= end; i++) {
        bool curr = memtest((int)(i & 0xffffffff));
#ifdef VERBOSE
        writer.printIntHex(i);
        writer.printChar(' ');
        writer.println(curr ? yes : no);
#else
        if (curr != prev) {
            if (curr) {
                writer.printIntBase(i, 16, 2);
                writer.printChar('-');
            } else {
                writer.printIntBase(i, 16, 2);
                writer.printChar(',');
                writer.printChar(' ');
            }
        }
#endif
        prev = curr;
    }
    writer << "Done!" << endl;
}

void test_mem(Writer& writer) {
    int size = 4;
    char* a = (char*)alloc(size);
    char* b = (char*)alloc(size);
    writer.printIntBase((int)a, 16, 8);
    writer << ' ';
    writer.printIntBase((int)b, 16, 8);
    writer << endl;
    for (int i = 0; i < size; i++) {
        a[i] = (char)(i % 256);
    }
    for (int i = 0; i < size; i++) {
        b[i] = (char)((i + 5) % 250);
    }
    char error[] = "An error occured at ";
    for (int i = 0; i < size; i++) {
        if (a[i] != (char)(i % 256)) {
            writer.print(error);
            writer.printIntBase((int)(a + i), 16, 8);
            writer << endl;
            return;
        }
    }
    for (int i = 0; i < size; i++) {
        if (b[i] != (char)((i + 5) % 250)) {
            writer.print(error);
            writer.printIntBase((int)(b + i), 16, 8);
            writer << endl;
            return;
        }
    }
    writer << "No errors when reading to and writing from memory." << endl;
}
