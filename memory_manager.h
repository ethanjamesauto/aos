void* alloc(int size);

extern "C" void* alloc_static(int size);

class MemoryManager {

  public:
    MemoryManager(int address, int size);
};
