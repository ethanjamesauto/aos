void* alloc(int size);

extern "C" void* alloc_static(int size) asm ("alloc_static");

class MemoryManager {

  public:
    MemoryManager(int address, int size);
};
