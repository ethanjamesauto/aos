extern "C" int memtest(int loc) asm("memtest");
extern "C" unsigned char port_byte_in(unsigned short address) asm("port_byte_in");
extern "C" void port_byte_out(unsigned short address, char data) asm("port_byte_out");
extern "C" unsigned short port_word_in(unsigned short address) asm("port_word_in");
extern "C" void port_word_out(unsigned short address, unsigned short data) asm("port_word_out");
extern "C" unsigned int test_asm(unsigned int i) asm("test_asm");
