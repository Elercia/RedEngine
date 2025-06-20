#pragma once

#define RED_USE_MAP
#ifndef RED_USE_MAP
#include <map>
namespace red
{
template <typename KeyT, typename ValueT>
using Map = std::map<KeyT, ValueT>;
}

#else  // ifndef RED_USE_MAP

#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Core/Debug/DebugMacros.hpp"
#include "RedEngine/Core/Macros.hpp"
#include "RedEngine/Core/Memory/Macros.hpp"
#include "RedEngine/Math/Hash.hpp"
#include "RedEngine/Utils/Types.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

namespace red
{
template <typename T>
struct EqualsT
{
    static bool Equals(const T& l, const T& r)
    {
        return l == r;
    }
};

template <typename T>
struct HashT
{
    static uint64 HashType(const T& v)
    {
        return Hash(v);
    }
};

// Open Addressing hash-map with quadratic probing
template <typename KeyT, typename ValueT, typename HashOpT = HashT<KeyT>, typename EqualsOpT = EqualsT<KeyT> >
class Map
{
public:
    struct KeyValuePair
    {
        KeyT first;
        ValueT second;

        ValueT* operator->()
        {
            return &second;
        }

        const ValueT* operator->() const
        {
            return &second;
        }

        const ValueT* operator*() const
        {
            return &second;
        }
    };

    using size_type = uint32;
    using value_type = KeyValuePair;
    using ThisType = Map<KeyT, ValueT, HashOpT, EqualsOpT>;

    class const_iterator;

    class iterator
    {
    public:
        friend ThisType;

        iterator(ThisType* map, size_type index);

        iterator(const iterator& other) = default;
        iterator(iterator&& other) = default;
        iterator& operator=(const iterator& other) = default;
        iterator& operator=(iterator&& other) = default;

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

        value_type& operator*();
        const value_type& operator*() const;

        value_type* operator->();
        const value_type* operator->() const;

        iterator& operator++();    // ++i
        iterator operator++(int);  // i++

        iterator& operator--();    // --i
        iterator operator--(int);  // i--

        operator const_iterator()
        {
            return const_iterator(m_map, m_index);
        }

    private:
        ThisType* m_map{nullptr};
        size_type m_index{0};
    };

    class const_iterator : public iterator
    {
    public:
        const_iterator(const ThisType* map, size_type index);

        const_iterator(const const_iterator& other) = default;
        const_iterator(const_iterator&& other) = default;
        const_iterator& operator=(const const_iterator& other) = default;
        const_iterator& operator=(const_iterator&& other) = default;

        using iterator::operator==;
        using iterator::operator!=;

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

        using iterator::operator*;

        using iterator::operator->;

        using iterator::operator++;

        using iterator::operator--;
    };

    struct Bucket
    {
        bool used = false;
        bool erased = false;
        uint8 value[sizeof(value_type)];
    };

    Map();
    Map(std::initializer_list<value_type> list);
    ~Map();

    Map(const ThisType& other);
    Map(ThisType&& other);

    ThisType& operator=(const ThisType& other);
    ThisType& operator=(ThisType&& other);

    ValueT& at(const KeyT& key);
    const ValueT& at(const KeyT& key) const;

    ValueT& operator[](const KeyT& key);
    const ValueT& operator[](const KeyT& key) const;

    std::pair<iterator, bool> insert(const value_type& value);
    std::pair<iterator, bool> insert(value_type&& value);

    template <typename OtherKeyT>
    bool contains(const OtherKeyT& key) const;
    template <typename OtherKeyT>
    iterator find(const OtherKeyT& key);
    template <typename OtherKeyT>
    const_iterator find(const OtherKeyT& key) const;

    iterator erase(const_iterator pos);
    iterator erase(const KeyT& key);

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    size_type size() const;
    bool empty() const;
    void clear();

private:
    static constexpr double MaxLoadFactor = 0.75f;

    // Compare the current size with the capacity (currently numbers of elements inside the array)
    // May rehash the table if the load factor is too high or too low (and at this point, iterator are invalidated)
    void EnsureCapacity();
    void Rehash(size_type newCapacity);

    size_type FirstIndex() const;
    size_type GetNext(size_type index) const;
    size_type GetPrevious(size_type index) const;
    value_type* GetPair(size_type index) const;

    template <bool forInsertion, typename OtherKeyT>
    size_type GetIndexOf(const OtherKeyT& key, const Array<Bucket>& inside) const;

private:
    Array<Bucket> m_values;
    size_type m_size;
};

template <>
inline uint64 Hash(const TypeTraits& value)
{
    return (uint64)value.typeId;
}

template <>
struct HashT<TypeTraits>
{
    static uint64 HashType(const TypeTraitsId& v)
    {
        return (uint64)v;
    }

    static uint64 HashType(const TypeTraits& v)
    {
        return v.typeId;
    }

    static uint64 HashType(const StringView& v)
    {
        return GetTypeInfoFromTypeName(v).typeId;
    }
};

template <>
struct EqualsT<TypeTraits>
{
    static bool Equals(const TypeTraits& l, const TypeTraits& r)
    {
        return l == r;
    }

    static bool Equals(const TypeTraits& l, const StringView& r)
    {
        return l.name == r;
    }

    static bool Equals(const TypeTraits& l, const TypeTraitsId& r)
    {
        return l.typeId == r;
    }
};

}  // namespace red
#endif

#include "inl/Map.inl"