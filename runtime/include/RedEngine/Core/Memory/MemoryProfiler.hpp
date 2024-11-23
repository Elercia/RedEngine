#pragma once

#include "RedEngine/Utils/Types.hpp"

#include <utility>

#ifndef RED_MODULE_NAME
#define RED_MODULE_NAME "RedEngine"
#endif

#define red_malloc(size)       red::MemoryProfiler::Allocate(size, __LINE__, __FILE__, RED_MODULE_NAME)
#define red_realloc(ptr, size) red::MemoryProfiler::Realloc(ptr, size, __LINE__, __FILE__, RED_MODULE_NAME)
#define red_free(ptr)          red::MemoryProfiler::Free(ptr)

#define red_frame_malloc(size)  red::MemoryProfiler::FrameAllocate(size, __LINE__, __FILE__, RED_MODULE_NAME)
#define red_frame_realloc(size) red::MemoryProfiler::FrameRealloc(ptr, size, __LINE__, __FILE__, RED_MODULE_NAME)

#define red_new(T, ...) red::MemoryProfiler::New<T>(__LINE__, __FILE__, ##__VA_ARGS__)
#define red_delete(ptr)                   \
    {                                     \
        red::MemoryProfiler::Delete(ptr); \
        ptr = nullptr;                    \
    }

#define MemoryGuard (uint32)0xBAADF00D

namespace red
{
struct AllocationInfo
{
    uint64 size;
    const char* moduleName;

#ifdef RED_MEMORY_LEAK_TRACER
    int line;
    const char* file;
#endif

    AllocationInfo* next;
    AllocationInfo* previous;
};

struct MemoryUsageInfo
{
    uint64 currentlyAllocated = 0;
    uint64 peekAllocated = 0;

    uint64 currentAllocationCount = 0;
    uint64 peekAllocationCount = 0;
};

class MemoryProfiler
{
public:
    static void* Allocate(sizet size, int line, const char* file, const char* moduleName);
    static void* Realloc(void* ptr, sizet size, int line, const char* file, const char* moduleName);
    static void Free(void* ptr);

    static void* FrameAllocate(sizet size, int line, const char* file, const char* moduleName);
    static void* FrameRealloc(void* ptr, sizet size, int line, const char* file, const char* moduleName);

    template <typename T, typename... Args>
    static T* New(int line, const char* file, Args... args);

    template <typename T>
    static void Delete(T* ptr);

    static void InitAllocInfo(AllocationInfo* info);

    static MemoryUsageInfo GetUsage();
    static void ResetUsage();

private:
    MemoryProfiler() = default;
    ~MemoryProfiler() = default;

#ifdef RED_MEMORY_LEAK_TRACER
    static AllocationInfo* s_rootAllocation;
#endif
    static MemoryUsageInfo s_memoryUsage;
};
}  // namespace red

#include "inl/MemoryProfiler.inl"
