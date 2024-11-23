#pragma once

#include "RedEngine/Utils/Types.hpp"

#define RED_KiB 1024llu
#define RED_MiB RED_KiB * 1024llu
#define RED_GiB RED_MB * 1024llu

namespace red
{
struct PageAllocation
{
    void* ptr;
    uint32 size;
};

PageAllocation VirtualAlloc(uint32 size, const PageAllocation* previousAlloc);
void VirtualFree(const PageAllocation& ptr);
}  // namespace red