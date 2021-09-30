#include "writer.h"

extern "C" void start() asm("start");
void new_line(Writer& writer);
int fib(int n);
void print_cmos(Writer& writer);
void test_routine(Writer& writer);
void check_writeable(Writer& writer);
void test_mem(Writer& writer);
