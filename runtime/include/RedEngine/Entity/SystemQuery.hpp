#pragma once

#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Utils/Tuple.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

namespace red
{

struct ComponentQuery
{
};
// Internal use only
struct IncludeCompQuery
{
};
struct ExcludeCompQuery
{
};
struct WriteCompQuery
{
};
struct ReadCompQuery
{
};

// Query Including require the entity to have the COMP component.
// The system will not touch this component.
template <typename COMP>
struct Including : ComponentQuery, IncludeCompQuery
{
    static const bool MUST_INCLUDE = true;

    using Type = COMP;
};

// Query MustNoHave require the entity to not have the COMP component
template <typename COMP>
struct Excluding : ComponentQuery, ExcludeCompQuery
{
    static const bool MUST_INCLUDE = false;
    using Type = COMP;
};

// Query Writing require the entity to have the COMP component with write access right
template <typename COMP>
struct Writing : public Including<COMP>, WriteCompQuery
{
    static const bool WRITING = true;
    using Type = COMP;
};

// Query Reading require the entity to have the COMP component with read access right
template <typename COMP>
struct Reading : public Including<COMP>, ReadCompQuery
{
    static const bool WRITING = false;
    using Type = COMP;
};

// TODO Adding comp ?

template <std::size_t I = 0, typename TypeToCompare, typename... Tp>
inline constexpr typename std::enable_if<I == sizeof...(Tp), int>::type CountQueryType(
    const std::tuple<Tp...>&, const TypeToCompare&)  // Unused arguments are given no names.
{
    return 0;
}

template <std::size_t I = 0, typename TypeToCompare, typename... Tp>
    inline constexpr typename std::enable_if <
    I<sizeof...(Tp), int>::type CountQueryType(const std::tuple<Tp...>& t, const TypeToCompare& ttc)
{
    using DecayedType = std::decay<decltype(std::get<I>(t))>::type;
    return (std::is_base_of<TypeToCompare, DecayedType>::value ? 1 : 0) +
           CountQueryType<I + 1, TypeToCompare, Tp...>(t, ttc);
}

template <typename... QueriesTypes>
constexpr static int GetWritingQueryCount()
{
    constexpr Tuple<QueriesTypes...> t;
    constexpr WriteCompQuery noop;

    return CountQueryType(t, noop);
}

template <typename... QueriesTypes>
constexpr static int GetReadingQueryCount()
{
    constexpr Tuple<QueriesTypes...> t;
    constexpr ReadCompQuery noop;

    return CountQueryType(t, noop);
}

template <typename... QueriesTypes>
constexpr static int GetIncludeQueryCount()
{
    constexpr Tuple<QueriesTypes...> t;
    constexpr IncludeCompQuery noop;

    return CountQueryType(t, noop);
}

template <typename... QueriesTypes>
constexpr static int GetExcludeQueryCount()
{
    constexpr Tuple<QueriesTypes...> t;
    constexpr ExcludeCompQuery noop;

    return CountQueryType(t, noop);
}

template <typename FilterType, typename... QueriesTypes>
constexpr auto FilterComponentTypeTraits()
{
    return std::apply(
        [](auto... ts)
        {
            return std::tuple_cat(std::conditional_t<std::is_base_of<FilterType, std::decay<decltype(ts)>::type>::value,
                                                     Tuple<std::decay<decltype(ts)>::type>, Tuple<>>{}...);
        },
        std::tuple<QueriesTypes...>());
}

template <typename FilterType, typename... QueriesTypes>
constexpr auto FetchComponentTypeTraits()
{
    return std::apply([](auto&&... x) { return std::array{TypeInfo<std::decay<decltype(x)>::type::Type>()...}; },
                      FilterComponentTypeTraits<FilterType, QueriesTypes...>());
}

template <typename... QueriesTypes>
constexpr auto GetRequireComponentTypeTuple()
{
    return std::tuple_cat(
        Tuple<EntityId>(),
        std::apply([](auto&&... x) { return Tuple<std::add_pointer<std::decay<decltype(x)>::type::Type>::type...>(); },
                   FilterComponentTypeTraits<IncludeCompQuery, QueriesTypes...>()));
}

template <typename... QueriesTypes>
struct QueryWithWriteComponents
{
    using FilterType = WriteCompQuery;

    static constexpr ArrayView<const TypeTraits> GetWritingComponents()
    {
        return ArrayView<const TypeTraits>(writingComps.data(), (int)writingComps.size());
    }
    static constexpr int GetWritingComponentCount()
    {
        return GetWritingQueryCount<QueriesTypes...>();
    }

    static constexpr std::array<TypeTraits, GetWritingComponentCount()> writingComps =
        FetchComponentTypeTraits<FilterType, QueriesTypes...>();
};

struct QueryWithoutWriteComponents
{
    constexpr static ArrayView<const TypeTraits> GetWritingComponents()
    {
        return ArrayView<const TypeTraits>();
    }

    constexpr static int GetWritingComponentCount()
    {
        return 0;
    }
};

template <typename... QueriesTypes>
struct QueryWithReadComponents
{
    using FilterType = ReadCompQuery;

    static constexpr ArrayView<const TypeTraits> GetReadingComponents()
    {
        return ArrayView<const TypeTraits>(readingComps.data(), (int)readingComps.size());
    }
    static constexpr int GetReadingComponentCount()
    {
        return GetReadingQueryCount<QueriesTypes...>();
    }

    static constexpr std::array<TypeTraits, GetReadingComponentCount()> readingComps =
        FetchComponentTypeTraits<FilterType, QueriesTypes...>();
};

struct QueryWithoutReadComponents
{
    constexpr static ArrayView<const TypeTraits> GetReadingComponents()
    {
        return ArrayView<const TypeTraits>();
    }

    constexpr static int GetReadingComponentCount()
    {
        return 0;
    }
};

template <typename... QueriesTypes>
struct QueryWithIncludeComponents
{
    using FilterType = IncludeCompQuery;

    static constexpr ArrayView<const TypeTraits> GetIncludingComponents()
    {
        return ArrayView<const TypeTraits>(includingComps.data(), (int)includingComps.size());
    }
    static constexpr int GetIncludingComponentCount()
    {
        return GetIncludeQueryCount<QueriesTypes...>();
    }

    static constexpr std::array<TypeTraits, GetIncludingComponentCount()> includingComps =
        FetchComponentTypeTraits<FilterType, QueriesTypes...>();
};

struct QueryWithoutIncludeComponents
{
    constexpr static ArrayView<const TypeTraits> GetIncludingComponents()
    {
        return ArrayView<const TypeTraits>();
    }

    constexpr static int GetIncludingComponentCount()
    {
        return 0;
    }
};

template <typename... QueriesTypes>
struct QueryWithExcludeComponents
{
    using FilterType = ExcludeCompQuery;

    static constexpr ArrayView<const TypeTraits> GetExcludingComponents()
    {
        return ArrayView<const TypeTraits>(excludingComps.data(), (int)excludingComps.size());
    }

    static constexpr int GetExcludingComponentCount()
    {
        return GetExcludeQueryCount<QueriesTypes...>();
    }

    static constexpr std::array<TypeTraits, GetExcludingComponentCount()> excludingComps =
        FetchComponentTypeTraits<FilterType, QueriesTypes...>();
};

struct QueryWithoutExcludeComponents
{
    constexpr static ArrayView<const TypeTraits> GetExcludingComponents()
    {
        return ArrayView<const TypeTraits>();
    }

    constexpr static int GetExcludingComponentCount()
    {
        return 0;
    }
};

template <typename... QueriesTypes>
struct Query : std::conditional<GetWritingQueryCount<QueriesTypes...>() == 0, QueryWithoutWriteComponents,
                                QueryWithWriteComponents<QueriesTypes...>>::type,
               std::conditional<GetReadingQueryCount<QueriesTypes...>() == 0, QueryWithoutReadComponents,
                                QueryWithReadComponents<QueriesTypes...>>::type,
               std::conditional<GetIncludeQueryCount<QueriesTypes...>() == 0, QueryWithoutIncludeComponents,
                                QueryWithIncludeComponents<QueriesTypes...>>::type,
               std::conditional<GetExcludeQueryCount<QueriesTypes...>() == 0, QueryWithoutExcludeComponents,
                                QueryWithExcludeComponents<QueriesTypes...>>::type
{
    static_assert(sizeof...(QueriesTypes) > 0, "You should have a least one component inside a query !");
    static_assert(all_true<std::is_base_of<ComponentQuery, QueriesTypes>::value...>::value,
                  "System query made of something else than a component query type");
};
}  // namespace red