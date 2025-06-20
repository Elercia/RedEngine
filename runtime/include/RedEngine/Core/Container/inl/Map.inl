
#ifdef RED_USE_MAP

namespace red
{
template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::iterator(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::ThisType* map,
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type index)
    : m_map(map), m_index(index)
{
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator::const_iterator(
    const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::ThisType* map,
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type index)
    : iterator(const_cast<typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::ThisType*>(map), index)
{
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
bool Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator==(
    const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator& other) const
{
    return m_index == other.m_index;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
bool Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator!=(
    const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator& other) const
{
    return !(*this == other);
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type& Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator*()
{
    return *m_map->GetPair(m_index);
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type&
Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator*() const
{
    return *m_map->GetPair(m_index);
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator& Map<KeyT, ValueT, HashOpT,
                                                              EqualsOpT>::iterator::operator++()  // ++i
{
    m_index = m_map->GetNext(m_index);
    return *this;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator++(
    int)  // i++
{
    iterator other = *this;
    m_index = m_map->GetNext(m_index);
    return other;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator& Map<KeyT, ValueT, HashOpT,
                                                              EqualsOpT>::iterator::operator--()  // --i
{
    m_index = m_map->GetPrevious(m_index);
    return *this;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator--(
    int)  // i--
{
    iterator other = *this;
    m_index = m_map->GetPrevious(m_index);
    return other;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type*
Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator->()
{
    return m_map->GetPair(m_index);
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type*
Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator::operator->() const
{
    return m_map->GetPair(m_index);
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
bool Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator::operator==(
    const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator& other) const
{
    return this->m_index == other.m_index;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
bool Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator::operator!=(
    const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator& other) const
{
    return !(*this == other);
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
Map<KeyT, ValueT, HashOpT, EqualsOpT>::Map() : m_values(), m_size(0)
{
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
Map<KeyT, ValueT, HashOpT, EqualsOpT>::Map(std::initializer_list<value_type> list) : m_values(), m_size(0)
{
    for (auto& value : list)
    {
        insert(value);
    }
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
Map<KeyT, ValueT, HashOpT, EqualsOpT>::~Map()
{
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
Map<KeyT, ValueT, HashOpT, EqualsOpT>::Map(const ThisType& other) : m_values(other.m_values), m_size(other.m_size)
{
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
Map<KeyT, ValueT, HashOpT, EqualsOpT>::Map(ThisType&& other)
    : m_values(std::move(other.m_values)), m_size(std::move(other.m_size))
{
    other.m_size = 0;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::ThisType& Map<KeyT, ValueT, HashOpT, EqualsOpT>::operator=(
    const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::ThisType& other)
{
    if (this == &other)
        return *this;

    m_values = other.m_values;
    m_size = other.m_size;

    return *this;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::ThisType& Map<KeyT, ValueT, HashOpT, EqualsOpT>::operator=(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::ThisType&& other)
{
    if (this == &other)
        return *this;

    m_values = std::move(other.m_values);
    m_size = std::move(other.m_size);

    other.m_size = 0;

    return *this;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
ValueT& Map<KeyT, ValueT, HashOpT, EqualsOpT>::at(const KeyT& key)
{
    return find(key)->second;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
const ValueT& Map<KeyT, ValueT, HashOpT, EqualsOpT>::at(const KeyT& key) const
{
    return find(key)->second;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
ValueT& Map<KeyT, ValueT, HashOpT, EqualsOpT>::operator[](const KeyT& key)
{
    auto it = find(key);
    if (it == end())
    {
        it = insert({key, ValueT()}).first;
    }

    return it->second;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
const ValueT& Map<KeyT, ValueT, HashOpT, EqualsOpT>::operator[](const KeyT& key) const
{
    return find(key)->second;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
std::pair<typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator, bool> Map<KeyT, ValueT, HashOpT, EqualsOpT>::insert(
    const typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type& value)
{
    EnsureCapacity();

    auto index = GetIndexOf<true>(value.first, m_values);
    bool added = true;
    auto& bucket = m_values[index];
    if (bucket.used)
    {
        added = false;
    }
    else
    {
        m_size++;
        new (bucket.value) value_type(value);
        bucket.used = true;
        bucket.erased = false;
    }

    return {iterator(this, index), added};
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
std::pair<typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator, bool> Map<KeyT, ValueT, HashOpT, EqualsOpT>::insert(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type&& value)
{
    EnsureCapacity();

    auto index = GetIndexOf<true>(value.first, m_values);
    bool added = true;
    auto& bucket = m_values[index];
    if (bucket.used)
    {
        added = false;
    }
    else
    {
        m_size++;
        new (bucket.value) value_type( std::move(value) );
        bucket.used = true;
        bucket.erased = false;
    }

    return {iterator(this, index), added};
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
template <typename OtherKeyT>
bool Map<KeyT, ValueT, HashOpT, EqualsOpT>::contains(const OtherKeyT& key) const
{
    return find(key) != end();
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
template <typename OtherKeyT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::find(const OtherKeyT& key)
{
    auto index = GetIndexOf<false>(key, m_values);
    if (index == (size_type) -1)
        return end();

    auto& bucket = m_values[index];

    if (bucket.used)
    {
        return iterator(this, index);
    }

    return end();
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
template <typename OtherKeyT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::find(
    const OtherKeyT& key) const
{
    auto index = GetIndexOf<false>(key, m_values);
    if (index == (size_type) -1)
        return end();

    auto& bucket = m_values[index];

    if (bucket.used)
    {
        return const_iterator(this, index);
    }

    return const_iterator(this, GetNext(index));
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::erase(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator pos)
{
    if (pos == end())
        return end();

    auto index = pos.m_index;
    if (index >= m_values.size())
        return end();

    auto& bucket = m_values[index];
    if (bucket.used)
    {
        bucket.used = false;
        bucket.erased = true;

        m_size--;

        ((value_type*) bucket.value)->~KeyValuePair();
    }

    return iterator(this, GetNext(index));
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::erase(const KeyT& key)
{
    auto index = GetIndexOf<false>(key, m_values);
    if (index == (size_type) -1)
        return end();

    auto& bucket = m_values[index];
    if (bucket.used)
    {
        bucket.used = false;
        bucket.erased = true;

        m_size--;

        ((value_type*) bucket.value)->~KeyValuePair();
    }

    return iterator(this, GetNext(index));
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::begin()
{
    return iterator(this, FirstIndex());
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::begin() const
{
    return const_iterator(this, FirstIndex());
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::cbegin() const
{
    return const_iterator(this, FirstIndex());
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::end()
{
    return iterator(this, m_values.size());
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::end() const
{
    return const_iterator(this, m_values.size());
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::const_iterator Map<KeyT, ValueT, HashOpT, EqualsOpT>::cend() const
{
    return const_iterator(this, m_values.size());
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type Map<KeyT, ValueT, HashOpT, EqualsOpT>::size() const
{
    return m_size;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
bool Map<KeyT, ValueT, HashOpT, EqualsOpT>::empty()  const
{
    return m_values.empty();
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
void Map<KeyT, ValueT, HashOpT, EqualsOpT>::clear()
{
    m_values.clear();
    m_values.shrink_to_fit();
    m_size = 0;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type Map<KeyT, ValueT, HashOpT, EqualsOpT>::FirstIndex() const
{
    for (size_type index = 0; index < m_values.size(); index++)
    {
        if (m_values[index].used)
            return index;
    }

    return m_values.size();
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type Map<KeyT, ValueT, HashOpT, EqualsOpT>::GetNext(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type index) const
{
    for (size_type i = index + 1; i < m_values.size(); i++)
    {
        const Bucket& bucket = m_values[i];
        if (bucket.used)
        {
            return i;
        }
    }

    return m_values.size();  // end
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type Map<KeyT, ValueT, HashOpT, EqualsOpT>::GetPrevious(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type index) const
{
    if (index == 0)
        return 0;

    size_type i = index - 1;
    do
    {
        const Bucket& bucket = m_values[i];
        if (bucket.used)
        {
            return i;
        }
    } while (i != 0);

    return index;  // first used
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type* Map<KeyT, ValueT, HashOpT, EqualsOpT>::GetPair(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type index) const
{
    const Bucket& bucket = m_values[index];
    RedAssert(bucket.used);

    const value_type* val = (const value_type*) bucket.value;

    return const_cast<typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::value_type*>(val);
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
template <bool forInsertion, typename OtherKeyT>
typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type Map<KeyT, ValueT, HashOpT, EqualsOpT>::GetIndexOf(
    const OtherKeyT& key, const Array<Bucket>& inside) const
{
    if (m_values.empty())
        return (size_type) -1;

    auto hash = HashOpT::HashType(key);

    auto index = (size_type) (hash % (uint64) inside.size());

    size_type firstErasedBucket = (size_type) -1;
    int j = 1;
    auto* bucket = &inside[index];
    while (true)
    {
        if (!bucket->used || EqualsOpT::Equals(((value_type*) bucket->value)->first, key))
            break;

        if constexpr (forInsertion)
        {
            if (firstErasedBucket != (size_type) -1 && bucket->erased)
                firstErasedBucket = index;
        }

        index = (index + (j * j)) % inside.size();  // Quadratic jumps
        j++;
        bucket = &inside[index];
    }

    if constexpr (forInsertion)
    {
        if (!bucket->used && firstErasedBucket != (size_type) -1)
            return firstErasedBucket;
    }

    RedAssert(index < inside.size());

    return index;
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
void Map<KeyT, ValueT, HashOpT, EqualsOpT>::EnsureCapacity()
{
    size_type newCapacity = 8;
    bool rehash = true;

    if (!m_values.empty())
    {
        rehash = false;
        const double loadFactor = (double) m_size / (double) m_values.size();

        if (loadFactor > MaxLoadFactor)
        {
            // Reduce capacity
            rehash = true;
            newCapacity = m_values.size() * 2;
        }
    }

    if (rehash)
    {
        Rehash(newCapacity);
    }
}

template <typename KeyT, typename ValueT, typename HashOpT, typename EqualsOpT>
void Map<KeyT, ValueT, HashOpT, EqualsOpT>::Rehash(
    typename Map<KeyT, ValueT, HashOpT, EqualsOpT>::size_type newCapacity)
{
    Array<Bucket> newBuckets;
    newBuckets.resize(newCapacity);

    for (auto& currentBucket : m_values)
    {
        if (currentBucket.used)
        {
            value_type* val = ((value_type*) currentBucket.value);
            auto newIndex = GetIndexOf<true>(val->first, newBuckets);

            Bucket& newBucket = newBuckets[newIndex];
            newBucket.used = true;
            new (newBucket.value) value_type(std::move(*val));
        }
    }

    m_values = std::move(newBuckets);
}

}  // namespace red

#endif