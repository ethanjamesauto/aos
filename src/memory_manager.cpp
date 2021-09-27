#include "memory_manager.h"

void* alloc(int size) {
    return alloc_static(size);
}