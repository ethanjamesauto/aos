extern "C" int memtest(int loc);
extern "C" unsigned char port_byte_in(unsigned short address);
extern "C" void port_byte_out(unsigned short address, char data);
extern "C" unsigned short port_word_in(unsigned short address);
extern "C" void port_word_out(unsigned short address, unsigned short data);
extern "C" unsigned int test_asm(unsigned int i);

