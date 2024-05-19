#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Entity/System.hpp"

#include <catch2/catch.hpp>

#include "TestUtils/TestUtils.hpp"

using namespace red;

struct WriteComp1
{
};

struct WriteComp2
{
};

struct ReadComp
{
};

struct NoNoNo
{
};

struct YesYesYes
{
};

TEST_CASE("Create query", "[ECS]")
{
    using TestQuery =
        Query<Writing<WriteComp1>, Writing<WriteComp2>, Reading<ReadComp>, Excluding<NoNoNo>, Including<YesYesYes>>;
    static_assert(TestQuery::GetReadingComponentCount() == 1);
    static_assert(TestQuery::GetWritingComponentCount() == 2);
    static_assert(TestQuery::GetIncludingComponentCount() == 4);
    static_assert(TestQuery::GetExcludingComponentCount() == 1);

    static_assert(TestQuery::GetExcludingComponents().size() == 1);

    static_assert(std::find(TestQuery::GetReadingComponents().begin(), TestQuery::GetReadingComponents().end(),
                            TypeInfo<ReadComp>()) != TestQuery::GetReadingComponents().end());

    static_assert(std::find(TestQuery::GetWritingComponents().begin(), TestQuery::GetWritingComponents().end(),
                            TypeInfo<WriteComp1>()) != TestQuery::GetWritingComponents().end());
    static_assert(std::find(TestQuery::GetWritingComponents().begin(), TestQuery::GetWritingComponents().end(),
                            TypeInfo<WriteComp2>()) != TestQuery::GetWritingComponents().end());

    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<WriteComp1>()) != TestQuery::GetIncludingComponents().end());
    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<WriteComp2>()) != TestQuery::GetIncludingComponents().end());
    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<ReadComp>()) != TestQuery::GetIncludingComponents().end());
    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<YesYesYes>()) != TestQuery::GetIncludingComponents().end());

    static_assert(std::find(TestQuery::GetExcludingComponents().begin(), TestQuery::GetExcludingComponents().end(),
                            TypeInfo<NoNoNo>()) != TestQuery::GetExcludingComponents().end());

    using NoReadQuery = Query<Writing<WriteComp1>, Writing<WriteComp2>>;

    static_assert(NoReadQuery::GetReadingComponentCount() == 0);
    static_assert(NoReadQuery::GetWritingComponentCount() == 2);
    static_assert(NoReadQuery::GetIncludingComponentCount() == 2);
    static_assert(NoReadQuery::GetExcludingComponentCount() == 0);
}