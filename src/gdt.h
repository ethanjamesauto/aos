struct gdt {
    short seg_limit0;
    short base_address0;
    char base_address16;
    char access;
    char limit_flags;
    char base_address24;
    gdt(unsigned base_address, unsigned limit, char flags, char access);
} __attribute__((packed));

inline constexpr char getFlags(unsigned limit, bool size, bool gran) {
    return (gran << 7) + (size << 6) + limit;
}

inline constexpr char getAccess(char present, char privilege, bool descriptor, bool executable, bool dc, bool writeable, bool accessed) {
    return (present << 7) + (privilege << 5) + (descriptor << 4) + (executable << 3) + (dc << 2) + (writeable << 1) + accessed;
}

inline constexpr char getAccess(char privilege, bool descriptor, bool executable, bool dc, bool writeable) {
    return getAccess(1, privilege, descriptor, executable, dc, writeable, 0);
}
 