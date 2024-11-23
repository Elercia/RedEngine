#pragma once

#include "RedEngine/Utils/Types.hpp"

namespace red
{
struct DefaultAllocator
{
    void* Allocate(uint32 size);
    void Free(void* ptr);
    void* Realloc(void* ptr, uint32 oldSize, uint32 size);
};
}