#pragma once

#include "RedEngine/Core/Memory/MemoryUtils.hpp"
#include "RedEngine/Utils/Types.hpp"
#include "RedEngine/Utils/Uncopyable.hpp"

#include <utility>

namespace red
{
class VirtualPoolAllocator : public Uncopyable
{
public:
    VirtualPoolAllocator(uint32 sizeOfElement, int initialCapacity);
    ~VirtualPoolAllocator();

    VirtualPoolAllocator(VirtualPoolAllocator&& other);
    VirtualPoolAllocator& operator=(VirtualPoolAllocator&& other);

    void Realloc(uint32 newCapacity);

    void* AllocateElement();
    void FreeElement(void* ptr);

    template <typename T, typename... Args>
    T* Allocate(Args&&... args);

    template <typename T>
    void Free(T* ptr);

private:
    void Grow();

private:
    struct FreeBlock
    {
        FreeBlock* nextFreeBlock = nullptr;
    };

    uint32 m_blockSize;
    uint32 m_capacity;
    uint32 m_currentAllocationCount;
    PageAllocation m_pageAlloc;

    FreeBlock* m_nextFreeBlock;
};
}  // namespace red

#include "inl/PoolAllocator.inl"