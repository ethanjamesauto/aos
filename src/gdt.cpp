#include "gdt.h"

gdt::gdt(unsigned base_address, unsigned limit, char limit_flags, char access) {
    this->seg_limit0 = limit & 0xffff;
    this->base_address0 = base_address & 0xffff;
    this->base_address16 = (base_address >> 16) & 0xff;
    this->access = access;
    this->limit_flags = limit_flags;
    this->base_address24 = base_address >> 24;
}
