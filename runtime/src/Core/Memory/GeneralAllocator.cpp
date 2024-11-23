#include "RedEngine/Core/Memory/GeneralAllocator.hpp"

#include "RedEngine/Core/CoreModule.hpp"

#include "RedEngine/Core/Memory/MemoryProfiler.hpp"

namespace red
{
void* DefaultAllocator::Allocate(uint32 size)
{
    return red_malloc(size);
}

void DefaultAllocator::Free(void* ptr)
{
    red_free(ptr);
}

void* DefaultAllocator::Realloc(void* ptr, uint32 /*oldSize*/, uint32 size)
{
    return red_realloc(ptr, size);
}
}