// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include "iceoryx_hoofs/testing/lifetime_and_assignment_tracker.hpp"
#include "iox/detail/hoofs_error_reporting.hpp"
#include "iox/vector.hpp"

#include "iceoryx_hoofs/testing/fatal_failure.hpp"
#include "test.hpp"

#include <vector>

namespace
{
using namespace ::testing;
using namespace iox;
using namespace iox::testing;

class vector_test : public Test
{
  public:
    using CTorTest = LifetimeAndAssignmentTracker<>;

    void SetUp() override
    {
        stats.reset();
    }

    CTorTest::Statistics& stats = CTorTest::stats;

    static constexpr uint64_t VECTOR_CAPACITY{10};
    vector<uint64_t, VECTOR_CAPACITY> sut;
};

TEST_F(vector_test, NewlyCreatedVectorIsEmpty)
{
     
    EXPECT_THAT(sut.empty(), Eq(true));
}

TEST_F(vector_test, NewlyCreatedVectorHasSizeZero)
{
     
    EXPECT_THAT(sut.size(), Eq(0U));
}

TEST_F(vector_test, Capacity)
{
     
    EXPECT_THAT(sut.capacity(), Eq(10U));
}

TEST_F(vector_test, NewVectorWithElementsCTorWithZeroElements)
{
     
    constexpr uint64_t CAPACITY{42U};
    constexpr int DEFAULT_VALUE{13};
    vector<int, CAPACITY> sut(0, DEFAULT_VALUE);
    EXPECT_THAT(sut.empty(), Eq(true));
}

TEST_F(vector_test, NewVectorWithElementsCTorWithSomeElements)
{
     
    constexpr uint64_t CAPACITY{42U};
    constexpr uint64_t ELEMENT_COUNT{37U};
    constexpr int DEFAULT_VALUE{13};
    vector<int, CAPACITY> sut(ELEMENT_COUNT, DEFAULT_VALUE);
    EXPECT_THAT(sut.size(), Eq(ELEMENT_COUNT));
    for (const auto& item : sut)
    {
        EXPECT_THAT(item, Eq(DEFAULT_VALUE));
    }
}

TEST_F(vector_test, NewVectorWithElementsCTorWithCapacityElements)
{
     
    constexpr uint64_t CAPACITY{42U};
    constexpr int DEFAULT_VALUE{13};
    vector<int, CAPACITY> sut(CAPACITY, DEFAULT_VALUE);
    EXPECT_THAT(sut.size(), Eq(CAPACITY));
    for (const auto& item : sut)
    {
        EXPECT_THAT(item, Eq(DEFAULT_VALUE));
    }
}

TEST_F(vector_test, NewVectorWithElementsCTorWithMoreThanCapacityElements)
{
     
    constexpr uint64_t CAPACITY{42U};
    constexpr uint64_t ELEMENT_COUNT{73U};
    constexpr int DEFAULT_VALUE{13};
    vector<int, CAPACITY> sut(ELEMENT_COUNT, DEFAULT_VALUE);
    EXPECT_THAT(sut.size(), Eq(CAPACITY));
    for (const auto& item : sut)
    {
        EXPECT_THAT(item, Eq(DEFAULT_VALUE));
    }
}

TEST_F(vector_test, EmplaceBackSuccessfulWhenSpaceAvailable)
{
     
    EXPECT_THAT(sut.emplace_back(5U), Eq(true));
}

TEST_F(vector_test, EmplaceBackFailsWhenSpaceNotAvailable)
{
     
    for (uint64_t i = 0U; i < 10U; ++i)
    {
        EXPECT_THAT(sut.emplace_back(5U), Eq(true));
    }
    EXPECT_THAT(sut.emplace_back(5U), Eq(false));
}

TEST_F(vector_test, PushBackSuccessfulWhenSpaceAvailableLValue)
{
     
    const int a{5};
    EXPECT_THAT(sut.push_back(a), Eq(true));
    ASSERT_THAT(sut.size(), Eq(1U));
    EXPECT_THAT(sut.at(0), Eq(a));
}

TEST_F(vector_test, PushBackFailsWhenSpaceNotAvailableLValue)
{
     
    const int a{5};
    for (uint64_t i = 0U; i < 10U; ++i)
    {
        EXPECT_THAT(sut.push_back(a), Eq(true));
    }
    EXPECT_THAT(sut.push_back(a), Eq(false));
}

TEST_F(vector_test, PushBackSuccessfulWhenSpaceAvailableRValue)
{
     
    EXPECT_THAT(sut.push_back(5U), Eq(true));
    ASSERT_THAT(sut.size(), Eq(1U));
    EXPECT_THAT(sut.at(0), Eq(5U));
}

TEST_F(vector_test, PushBackFailsWhenSpaceNotAvailableRValue)
{
     
    for (uint64_t i = 0U; i < 10U; ++i)
    {
        EXPECT_THAT(sut.push_back(5U), Eq(true));
    }
    EXPECT_THAT(sut.push_back(5U), Eq(false));
}

TEST_F(vector_test, PopBackOnEmptyVector)
{
     
    sut.pop_back();
    ASSERT_THAT(sut.size(), Eq(0U));
}

TEST_F(vector_test, PopBackNonEmptyVector)
{
     
    vector<CTorTest, 10U> sut;
    sut.emplace_back(101U);
    ASSERT_THAT(sut.size(), Eq(1U));
    stats.dTor = 0;
    sut.pop_back();
    ASSERT_THAT(sut.size(), Eq(0U));
    ASSERT_THAT(stats.dTor, Eq(1));
}

TEST_F(vector_test, SizeIncreasesWhenElementIsAdded)
{
     
    sut.emplace_back(5U);
    EXPECT_THAT(sut.size(), Eq(1U));
}

TEST_F(vector_test, SizeEqualsCapacityWheFull)
{
     
    for (uint64_t i = 0U; i < 10U; ++i)
    {
        sut.emplace_back(5U);
    }
    EXPECT_THAT(sut.size(), Eq(sut.capacity()));
}

TEST_F(vector_test, SizeUnchangedWhenEmplaceFails)
{
     
    for (uint64_t i = 0U; i < 10U; ++i)
    {
        sut.emplace_back(5U);
    }
    EXPECT_THAT(sut.emplace_back(5U), Eq(false));
    EXPECT_THAT(sut.size(), Eq(sut.capacity()));
}

TEST_F(vector_test, NotEmptyWhenElementWasAdded)
{
     
    sut.emplace_back(5U);
    EXPECT_THAT(sut.empty(), Eq(false));
}

TEST_F(vector_test, EmptyAfterClear)
{
     
    sut.emplace_back(5U);
    sut.clear();
    EXPECT_THAT(sut.empty(), Eq(true));
}

TEST_F(vector_test, SizeZeroAfterClear)
{
     
    sut.emplace_back(5U);
    sut.clear();
    EXPECT_THAT(sut.size(), Eq(0U));
}

TEST_F(vector_test, CopyConstructor)
{
     
    vector<CTorTest, 10U> sut1;
    sut1.emplace_back(101U);
    sut1.emplace_back(102U);

    vector<CTorTest, 10> sut2(sut1);
    EXPECT_THAT(stats.copyCTor, Eq(2U));
    EXPECT_THAT(sut2.at(0).value, Eq(101U));
    EXPECT_THAT(sut2.at(1).value, Eq(102U));
    EXPECT_THAT(sut2.empty(), Eq(false));
    EXPECT_THAT(sut2.size(), Eq(2));
}

TEST_F(vector_test, CopyConstructorWithEmptyVector)
{
     
    vector<CTorTest, 10U> sut1;
    // NOLINTJUSTIFICATION Testing empty copy
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    vector<CTorTest, 10> sut2(sut1);
    EXPECT_THAT(stats.copyCTor, Eq(0U));
    EXPECT_THAT(sut2.size(), Eq(0U));
    EXPECT_THAT(sut2.empty(), Eq(true));
}

TEST_F(vector_test, CopyConstructorWithFullVector)
{
     
    vector<CTorTest, 10U> sut1;
    for (uint64_t i = 0U; i < 10U; ++i)
    {
        sut1.emplace_back(i);
    }

    vector<CTorTest, 10> sut2(sut1);
    for (uint64_t i = 0; i < 10; ++i)
    {
        EXPECT_THAT(sut2.at(i).value, Eq(i));
    }

    EXPECT_THAT(stats.copyCTor, Eq(10U));
    EXPECT_THAT(sut2.size(), Eq(10U));
    EXPECT_THAT(sut2.empty(), Eq(false));
}

TEST_F(vector_test, MoveConstructor)
{
     
    vector<CTorTest, 10U> sut1;
    sut1.emplace_back(8101U);
    sut1.emplace_back(8102U);

    vector<CTorTest, 10U> sut2(std::move(sut1));

    EXPECT_THAT(stats.moveCTor, Eq(2U));
    EXPECT_THAT(sut2.at(0).value, Eq(8101U));
    EXPECT_THAT(sut2.at(1).value, Eq(8102U));
    EXPECT_THAT(sut2.empty(), Eq(false));
    EXPECT_THAT(sut2.size(), Eq(2U));
}

TEST_F(vector_test, MoveConstructorWithEmptyVector)
{
     
    vector<CTorTest, 10U> sut1;

    vector<CTorTest, 10U> sut2(std::move(sut1));

    EXPECT_THAT(stats.moveCTor, Eq(0U));
    EXPECT_THAT(sut2.size(), Eq(0U));
    EXPECT_THAT(sut2.empty(), Eq(true));
}

TEST_F(vector_test, MoveConstructorWithFullVector)
{
     
    vector<CTorTest, 10U> sut1;
    for (uint64_t i = 0U; i < 10U; ++i)
    {
        sut1.emplace_back(i);
    }

    vector<CTorTest, 10U> sut2(std::move(sut1));

    for (uint64_t i = 0; i < 10; ++i)
    {
        EXPECT_THAT(sut2.at(i).value, Eq(i));
    }

    EXPECT_THAT(stats.moveCTor, Eq(10U));
    EXPECT_THAT(sut2.size(), Eq(10U));
    EXPECT_THAT(sut2.empty(), Eq(false));
}

TEST_F(vector_test, DestructorWithEmptyVector)
{
     
    {
        vector<CTorTest, 10U> sut1;
    }
    EXPECT_THAT(stats.dTor, Eq(0U));
}

TEST_F(vector_test, DestructorSomeElements)
{
     
    {
        vector<CTorTest, 10U> sut1;
        sut1.emplace_back(891U);
        sut1.emplace_back(9191U);
        sut1.emplace_back(1U);
    }
    EXPECT_THAT(stats.dTor, Eq(3U));
}

TEST_F(vector_test, DestructorWithFullVector)
{
     
    constexpr uint64_t CAPACITY{10};
    {
        vector<CTorTest, CAPACITY> sut1;
        for (uint64_t i = 0U; i < CAPACITY; ++i)
        {
            sut1.emplace_back(1231U);
        }
    }

    EXPECT_THAT(stats.dTor, Eq(CAPACITY));
}

TEST_F(vector_test, CopyAssignmentWithEmptySource)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(812U);
    sut1.emplace_back(81122U);
    sut1.emplace_back(8132U);

    sut1 = sut2;

    EXPECT_THAT(stats.dTor, Eq(3U));
    EXPECT_THAT(stats.copyAssignment, Eq(0U));
    EXPECT_THAT(stats.copyCTor, Eq(0U));
    EXPECT_THAT(sut1.size(), Eq(0U));
    EXPECT_THAT(sut1.empty(), Eq(true));
}

TEST_F(vector_test, CopyAssignmentWithEmptyDestination)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(5812U);
    sut1.emplace_back(581122U);
    sut1.emplace_back(58132U);

    sut2 = sut1;

    EXPECT_THAT(stats.dTor, Eq(0U));
    EXPECT_THAT(stats.copyAssignment, Eq(0U));
    EXPECT_THAT(stats.copyCTor, Eq(3U));
    EXPECT_THAT(sut2.size(), Eq(3U));
    EXPECT_THAT(sut2.empty(), Eq(false));

    EXPECT_THAT(sut2.at(0U).value, Eq(5812U));
    EXPECT_THAT(sut2.at(1U).value, Eq(581122U));
    EXPECT_THAT(sut2.at(2U).value, Eq(58132U));
}

TEST_F(vector_test, CopyAssignmentWithLargerDestination)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(5842U);
    sut1.emplace_back(584122U);
    sut1.emplace_back(58432U);
    sut1.emplace_back(58432U);
    sut2.emplace_back(313U);
    sut2.emplace_back(3131U);

    sut1 = sut2;

    EXPECT_THAT(stats.dTor, Eq(2U));
    EXPECT_THAT(stats.copyAssignment, Eq(2U));
    EXPECT_THAT(stats.copyCTor, Eq(0U));
    EXPECT_THAT(sut1.size(), Eq(2U));
    EXPECT_THAT(sut1.empty(), Eq(false));
    EXPECT_THAT(sut1.at(0U).value, Eq(313U));
    EXPECT_THAT(sut1.at(1U).value, Eq(3131U));
}

TEST_F(vector_test, CopyAssignmentWithLargerSource)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(15842U);
    sut1.emplace_back(1584122U);
    sut1.emplace_back(158432U);
    sut1.emplace_back(158432U);
    sut2.emplace_back(1313U);
    sut2.emplace_back(13131U);

    sut2 = sut1;

    EXPECT_THAT(stats.dTor, Eq(0U));
    EXPECT_THAT(stats.copyAssignment, Eq(2U));
    EXPECT_THAT(stats.copyCTor, Eq(2U));
    EXPECT_THAT(sut2.size(), Eq(4U));
    EXPECT_THAT(sut2.empty(), Eq(false));
    EXPECT_THAT(sut2.at(0U).value, Eq(15842U));
    EXPECT_THAT(sut2.at(1U).value, Eq(1584122U));
    EXPECT_THAT(sut2.at(2U).value, Eq(158432U));
    EXPECT_THAT(sut2.at(3U).value, Eq(158432U));
}

TEST_F(vector_test, ReverseDestructionOrderInCopyAssignment)
{
     
    constexpr uint64_t VECTOR_CAPACITY{10};
    vector<CTorTest, VECTOR_CAPACITY> sut1;
    vector<CTorTest, VECTOR_CAPACITY> sut2;
    for (uint64_t i{0}; i < VECTOR_CAPACITY; ++i)
    {
        sut1.emplace_back(i);
    }
    sut1 = sut2;

    EXPECT_THAT(stats.dTor, Eq(VECTOR_CAPACITY));
    ASSERT_THAT(stats.dTorOrder.size(), Eq(VECTOR_CAPACITY));
    for (size_t i{0}; i < VECTOR_CAPACITY; ++i)
    {
        EXPECT_THAT(stats.dTorOrder[i], Eq(VECTOR_CAPACITY - 1 - i));
    }
}

TEST_F(vector_test, ReverseDestructionOrderInMoveAssignment)
{
     
    constexpr uint64_t VECTOR_CAPACITY{10};
    vector<CTorTest, VECTOR_CAPACITY> sut1;
    vector<CTorTest, VECTOR_CAPACITY> sut2;
    for (uint64_t i{0}; i < VECTOR_CAPACITY; ++i)
    {
        sut1.emplace_back(i + 1);
    }
    sut1 = std::move(sut2);

    EXPECT_THAT(stats.dTor, Eq(VECTOR_CAPACITY));
    ASSERT_THAT(stats.dTorOrder.size(), Eq(VECTOR_CAPACITY));
    for (size_t i{0}; i < VECTOR_CAPACITY; ++i)
    {
        EXPECT_THAT(stats.dTorOrder[i], Eq(VECTOR_CAPACITY - i));
    }
}

TEST_F(vector_test, MoveAssignmentWithEmptySource)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(812U);
    sut1.emplace_back(81122U);
    sut1.emplace_back(8132U);

    sut1 = std::move(sut2);

    EXPECT_THAT(stats.dTor, Eq(3U));
    EXPECT_THAT(stats.moveAssignment, Eq(0U));
    EXPECT_THAT(stats.moveCTor, Eq(0U));
    EXPECT_THAT(sut1.size(), Eq(0U));
    EXPECT_THAT(sut1.empty(), Eq(true));
}

TEST_F(vector_test, MoveAssignmentWithEmptyDestination)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(5812U);
    sut1.emplace_back(581122U);
    sut1.emplace_back(58132U);

    sut2 = std::move(sut1);

    EXPECT_THAT(stats.dTor, Eq(3U));
    EXPECT_THAT(stats.moveAssignment, Eq(0U));
    EXPECT_THAT(stats.moveCTor, Eq(3U));
    EXPECT_THAT(sut2.size(), Eq(3U));
    EXPECT_THAT(sut2.empty(), Eq(false));
    EXPECT_THAT(sut2.at(0U).value, Eq(5812U));
    EXPECT_THAT(sut2.at(1U).value, Eq(581122U));
    EXPECT_THAT(sut2.at(2U).value, Eq(58132U));
}

TEST_F(vector_test, MoveAssignmentWithLargerDestination)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(5842U);
    sut1.emplace_back(584122U);
    sut1.emplace_back(58432U);
    sut1.emplace_back(58432U);
    sut2.emplace_back(313U);
    sut2.emplace_back(3131U);

    sut1 = std::move(sut2);

    EXPECT_THAT(stats.dTor, Eq(4U));
    EXPECT_THAT(stats.moveAssignment, Eq(2U));
    EXPECT_THAT(stats.moveCTor, Eq(0U));
    EXPECT_THAT(sut1.size(), Eq(2U));
    EXPECT_THAT(sut1.empty(), Eq(false));
    EXPECT_THAT(sut1.at(0U).value, Eq(313U));
    EXPECT_THAT(sut1.at(1U).value, Eq(3131U));
}

TEST_F(vector_test, MoveAssignmentWithLargerSource)
{
     
    vector<CTorTest, 10U> sut1;
    vector<CTorTest, 10U> sut2;
    sut1.emplace_back(15842U);
    sut1.emplace_back(1584122U);
    sut1.emplace_back(158432U);
    sut1.emplace_back(158432U);
    sut2.emplace_back(1313U);
    sut2.emplace_back(13131U);

    sut2 = std::move(sut1);

    EXPECT_THAT(stats.dTor, Eq(4U));
    EXPECT_THAT(stats.moveAssignment, Eq(2U));
    EXPECT_THAT(stats.moveCTor, Eq(2U));
    EXPECT_THAT(sut2.size(), Eq(4U));
    EXPECT_THAT(sut2.empty(), Eq(false));
    EXPECT_THAT(sut2.at(0U).value, Eq(15842U));
    EXPECT_THAT(sut2.at(1U).value, Eq(1584122U));
    EXPECT_THAT(sut2.at(2U).value, Eq(158432U));
    EXPECT_THAT(sut2.at(3U).value, Eq(158432U));
}

TEST_F(vector_test, BeginEndIteratorAreTheSameWhenEmpty)
{
     
    EXPECT_THAT(sut.begin() == sut.end(), Eq(true));
}

TEST_F(vector_test, BeginEndConstIteratorAreTheSameWhenEmpty)
{
     
    // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->begin() == const_cast<const decltype(sut)*>(&sut)->end(),
                Eq(true));
}

TEST_F(vector_test, BeginIteratorComesBeforeEndIteratorWhenNotEmpty)
{
     
    sut.emplace_back(1U);
    EXPECT_THAT(sut.begin() < sut.end(), Eq(true));
}

TEST_F(vector_test, BeginConstIteratorComesBeforeEndConstIteratorWhenNotEmpty)
{
     
    sut.emplace_back(1U);
    // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->begin() < const_cast<const decltype(sut)*>(&sut)->end(),
                Eq(true));
}

TEST_F(vector_test, BeginIteratorComesBeforeEndIteratorWhenFull)
{
     
    for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        sut.emplace_back(i);
    }
    EXPECT_THAT(sut.begin() < sut.end(), Eq(true));
}

TEST_F(vector_test, BeginConstIteratorComesBeforeEndConstIteratorWhenFull)
{
     
    for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        sut.emplace_back(i);
    }
    // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->begin() < const_cast<const decltype(sut)*>(&sut)->end(),
                Eq(true));
}

TEST_F(vector_test, IteratorIteratesThroughNonEmptyVector)
{
     
    constexpr uint64_t INITIAL_VALUE = 42U;
    sut.emplace_back(INITIAL_VALUE);
    sut.emplace_back(INITIAL_VALUE + 1U);
    sut.emplace_back(INITIAL_VALUE + 2U);
    const uint64_t EXPECTED_END_INDEX = sut.size();

    uint64_t count = 0U;
    for (auto& v : sut)
    {
        EXPECT_THAT(v, Eq(INITIAL_VALUE + count));
        ++count;
    }
    EXPECT_THAT(count, Eq(EXPECTED_END_INDEX));
}

TEST_F(vector_test, ConstIteratorIteratesThroughNonEmptyVector)
{
     
    constexpr uint64_t INITIAL_VALUE{142U};
    sut.emplace_back(INITIAL_VALUE);
    sut.emplace_back(INITIAL_VALUE + 1U);
    sut.emplace_back(INITIAL_VALUE + 2U);
    const uint64_t EXPECTED_END_COUNT = sut.size();

    uint64_t count = 0U;
    // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    for (const auto& v : *const_cast<const decltype(sut)*>(&sut))
    {
        EXPECT_THAT(v, Eq(INITIAL_VALUE + count));
        ++count;
    }
    EXPECT_THAT(count, Eq(EXPECTED_END_COUNT));
}

TEST_F(vector_test, IteratorIteratesThroughFullVector)
{
     
    for (uint64_t k = 0U; k < VECTOR_CAPACITY; ++k)
    {
        sut.emplace_back(42U * k);
    }
    const auto EXPECTED_END_COUNT = sut.size();

    int i = 0;
    for (auto& v : sut)
    {
        EXPECT_THAT(v, Eq(42 * (i++)));
    }
    EXPECT_THAT(i, Eq(EXPECTED_END_COUNT));
}

TEST_F(vector_test, ConstIteratorIteratesThroughFullVector)
{
     
    for (uint64_t k = 0U; k < VECTOR_CAPACITY; ++k)
    {
        sut.emplace_back(142U * k);
    }
    const auto EXPECTED_END_COUNT = sut.size();

    int i = 0;
    // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    for (const auto& v : *const_cast<const decltype(sut)*>(&sut))
    {
        EXPECT_THAT(v, Eq(142 * (i++)));
    }
    EXPECT_THAT(i, Eq(EXPECTED_END_COUNT));
}

TEST_F(vector_test, IterateUsingData)
{
     
    sut.emplace_back(127U);
    sut.emplace_back(128U);
    sut.emplace_back(129U);

    for (uint64_t k = 0U; k < sut.size(); ++k)
    {
        // NOLINTJUSTIFICATION Bounds considered
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        EXPECT_THAT(sut.data()[k], Eq(127U + k));
    }
}

TEST_F(vector_test, IterateUsingConstData)
{
     
    sut.emplace_back(3127U);
    sut.emplace_back(3128U);
    sut.emplace_back(3129U);

    for (uint64_t k = 0U; k < sut.size(); ++k)
    {
        // NOLINTJUSTIFICATION Bounds considered, const method tested
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic,cppcoreguidelines-pro-type-const-cast)
        EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->data()[k], Eq(3127U + k));
    }
}

TEST_F(vector_test, IterateUsingAt)
{
     
    sut.emplace_back(127U);
    sut.emplace_back(128U);
    sut.emplace_back(129U);

    for (uint64_t k = 0; k < sut.size(); ++k)
    {
        EXPECT_THAT(sut.at(k), Eq(127U + k));
    }
}

TEST_F(vector_test, IterateUsingConstAt)
{
     
    sut.emplace_back(3127U);
    sut.emplace_back(3128U);
    sut.emplace_back(3129U);

    for (uint64_t k = 0; k < sut.size(); ++k)
    {
        // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
        EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->at(k), Eq(3127U + k));
    }
}

TEST_F(vector_test, IterateUsingSquareBracket)
{
     
    sut.emplace_back(2127U);
    sut.emplace_back(2128U);
    sut.emplace_back(2129U);

    for (uint64_t k = 0; k < sut.size(); ++k)
    {
        EXPECT_THAT(sut[k], Eq(2127U + k));
    }
}

TEST_F(vector_test, IterateUsingConstSquareBracket)
{
     
    sut.emplace_back(4127U);
    sut.emplace_back(4128U);
    sut.emplace_back(4129U);

    for (uint64_t k = 0; k < sut.size(); ++k)
    {
        // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
        EXPECT_THAT((*const_cast<const decltype(sut)*>(&sut))[k], Eq(4127U + k));
    }
}

TEST_F(vector_test, EraseFailsWhenElementIsInvalid)
{
     
    auto* i = sut.begin() + 5U;
    EXPECT_FALSE(sut.erase(i));
    EXPECT_FALSE(sut.erase(sut.end()));
    EXPECT_FALSE(sut.erase(sut.begin() - 1));
}

TEST_F(vector_test, ErasingElementDecreasesSize)
{
     
    sut.emplace_back(3U);
    sut.emplace_back(4U);
    sut.emplace_back(5U);
    EXPECT_TRUE(sut.erase(sut.begin() + 2U));
    EXPECT_TRUE(sut.erase(sut.begin()));
    EXPECT_THAT(sut.size(), Eq(1U));
}

TEST_F(vector_test, EraseOfLastElementCallsDTorOnly)
{
     
    vector<CTorTest, 5U> sut1;
    sut1.emplace_back(7U);
    sut1.emplace_back(8U);
    sut1.emplace_back(9U);

    EXPECT_TRUE(sut1.erase(sut1.begin() + 2U));

    EXPECT_THAT(stats.dTor, Eq(1U));
    EXPECT_THAT(stats.classValue, Eq(9U));
}

TEST_F(vector_test, EraseOfMiddleElementCallsDTorAndMove)
{
     
    vector<CTorTest, 5U> sut1;
    sut1.emplace_back(7U);
    sut1.emplace_back(8U);
    sut1.emplace_back(9U);
    sut1.emplace_back(10U);
    sut1.emplace_back(11U);

    EXPECT_TRUE(sut1.erase(sut1.begin() + 2U));

    EXPECT_THAT(stats.dTor, Eq(1U));
    EXPECT_THAT(stats.moveAssignment, Eq(2U));
}

TEST_F(vector_test, AccessOfNonExistingElementOnEmptyVectorLeadTermination)
{
     

    ASSERT_THAT(sut.empty(), Eq(true));

    const uint64_t accessOffset{sut.size() + 1U};
    IOX_EXPECT_FATAL_FAILURE([&] { sut.at(accessOffset); }, iox::er::ENFORCE_VIOLATION);
}

TEST_F(vector_test, AccessOfNonExistingElementOnPartiallyFilledVectorLeadTermination)
{
     
    constexpr int a{5};

    for (uint64_t i = 0U; i < (VECTOR_CAPACITY - 2U); ++i)
    {
        ASSERT_THAT(sut.push_back(a), Eq(true));
    }

    const uint64_t accessOffset{sut.size() + 1U};
    IOX_EXPECT_FATAL_FAILURE([&] { sut.at(accessOffset); }, iox::er::ENFORCE_VIOLATION);
}

TEST_F(vector_test, AccessOfNonExistingElementOnFullVectorLeadTermination)
{
     
    constexpr int a{5};

    for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        ASSERT_THAT(sut.push_back(a), Eq(true));
    }

    ASSERT_THAT(sut.size(), Eq(VECTOR_CAPACITY));

    const uint64_t accessOffset{sut.size() + 1U};
    IOX_EXPECT_FATAL_FAILURE([&] { sut.at(accessOffset); }, iox::er::ENFORCE_VIOLATION);
}

TEST_F(vector_test, OutOfBoundsAccessOnEmptyVectorLeadsToTermination)
{
     

    ASSERT_THAT(sut.empty(), Eq(true));

    const uint64_t accessOffset{sut.size() + 1U};
    IOX_EXPECT_FATAL_FAILURE([&] { sut[accessOffset]; }, iox::er::ENFORCE_VIOLATION);
}

TEST_F(vector_test, OutOfBoundsAccessOnPartiallyFilledVectorLeadsToTermination)
{
     
    constexpr int a{5};

    for (uint64_t i = 0U; i < (VECTOR_CAPACITY - 2U); ++i)
    {
        ASSERT_THAT(sut.push_back(a), Eq(true));
    }

    const uint64_t accessOffset{sut.size() + 1U};
    IOX_EXPECT_FATAL_FAILURE([&] { sut[accessOffset]; }, iox::er::ENFORCE_VIOLATION);
}

TEST_F(vector_test, OutOfBoundsAccessOnFullVectorLeadsToTermination)
{
     
    constexpr int a{5};

    for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        ASSERT_THAT(sut.push_back(a), Eq(true));
    }

    const uint64_t accessOffset{sut.size() + 1U};
    IOX_EXPECT_FATAL_FAILURE([&] { sut[accessOffset]; }, iox::er::ENFORCE_VIOLATION);
}

TEST_F(vector_test, EraseOfFrontElementCallsDTorAndMove)
{
     
    vector<CTorTest, 5U> sut1;
    sut1.emplace_back(7U);
    sut1.emplace_back(8U);
    sut1.emplace_back(9U);
    sut1.emplace_back(10U);
    sut1.emplace_back(11U);

    EXPECT_TRUE(sut1.erase(sut1.begin()));

    EXPECT_THAT(stats.dTor, Eq(1U));
    EXPECT_THAT(stats.moveAssignment, Eq(4U));
}

TEST_F(vector_test, EraseMiddleElementDataCorrectAfterwards)
{
     
    sut.emplace_back(97U);
    sut.emplace_back(101U);
    sut.emplace_back(98U);
    sut.emplace_back(99U);

    EXPECT_TRUE(sut.erase(sut.begin() + 1U));

    for (uint64_t k = 0U; k < sut.size(); ++k)
    {
        EXPECT_THAT(sut[k], Eq(97U + k));
    }
}

TEST_F(vector_test, EraseFrontElementDataCorrectAfterwards)
{
     
    sut.emplace_back(6101U);
    sut.emplace_back(597U);
    sut.emplace_back(598U);
    sut.emplace_back(599U);

    EXPECT_TRUE(sut.erase(sut.begin()));

    for (uint64_t k = 0U; k < sut.size(); ++k)
    {
        EXPECT_THAT(sut[k], Eq(597U + k));
    }
}

TEST_F(vector_test, EraseLastElementDataCorrectAfterwards)
{
     
    sut.emplace_back(7597U);
    sut.emplace_back(7598U);
    sut.emplace_back(7599U);
    sut.emplace_back(7600U);
    sut.emplace_back(7601U);
    sut.emplace_back(76101U);

    EXPECT_TRUE(sut.erase(sut.begin() + 5U));

    for (uint64_t k = 0U; k < sut.size(); ++k)
    {
        EXPECT_THAT(sut[k], Eq(7597U + k));
    }
}

TEST_F(vector_test, EraseLastElementOfFullVectorDataCorrectAfterwards)
{
     
    for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        sut.emplace_back(i * 123U);
    }

    EXPECT_TRUE(sut.erase(sut.begin() + sut.size() - 1U));

    for (uint64_t k = 0; k < sut.size(); ++k)
    {
        EXPECT_THAT(sut[k], Eq(k * 123U));
    }
}

TEST_F(vector_test, FrontPointsToFirstElement)
{
     
    sut.emplace_back(1U);
    sut.emplace_back(2U);
    sut.emplace_back(3U);
    EXPECT_THAT(sut.front(), Eq(1));
}

TEST_F(vector_test, BackPointsToLastElement)
{
     
    sut.emplace_back(4U);
    sut.emplace_back(5U);
    sut.emplace_back(6U);
    EXPECT_THAT(sut.back(), Eq(6U));
}

TEST_F(vector_test, ConstFrontPointsToFirstElement)
{
     
    sut.emplace_back(7U);
    sut.emplace_back(8U);
    sut.emplace_back(9U);

    // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->front(), Eq(7U));
}

TEST_F(vector_test, ConstBackPointsToLastElement)
{
     
    sut.emplace_back(10U);
    sut.emplace_back(11U);
    sut.emplace_back(12U);

    // NOLINTJUSTIFICATION Re-use 'sut' and testing const methods
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->back(), Eq(12U));
}

TEST_F(vector_test, ConstructorWithSizeParameterSmallerThanCapacity)
{
     
    vector<CTorTest, 5U> sut(2U);
    EXPECT_THAT(stats.cTor, Eq(2U));
    ASSERT_THAT(sut.size(), Eq(2U));
}

TEST_F(vector_test, ConstructorWithSizeParameterGreaterThanCapacity)
{
     
    vector<CTorTest, 5U> sut(7U);
    EXPECT_THAT(stats.cTor, Eq(5U));
    ASSERT_THAT(sut.size(), Eq(5U));
}

TEST_F(vector_test, TwoEmptyVectorOfSameCapacityAreEqual)
{
     
    vector<int, 10> a;
    vector<int, 10> b;

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST_F(vector_test, TwoEmptyVectorOfDifferentCapacityAreEqual)
{
     
    vector<int, 10> a;
    vector<int, 20> b;

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST_F(vector_test, TwoEqualVectorsWithSameCapacityAreEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-6"); 
    vector<int, 10> a;
    vector<int, 10> b;

    a.emplace_back(1);
    a.emplace_back(2);
    a.emplace_back(3);
    b.emplace_back(1);
    b.emplace_back(2);
    b.emplace_back(3);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST_F(vector_test, TwoEqualVectorsWithDifferentCapacityAreEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-5"); 
    vector<int, 10> a;
    vector<int, 20> b;

    a.emplace_back(4);
    a.emplace_back(5);
    a.emplace_back(6);
    b.emplace_back(4);
    b.emplace_back(5);
    b.emplace_back(6);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST_F(vector_test, TwoVectorsWithDifferentSizeAndSameCapacityAreNotEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-2"); 
    vector<int, 10> a;
    vector<int, 10> b;

    a.emplace_back(7);
    a.emplace_back(8);
    a.emplace_back(9);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST_F(vector_test, TwoNonEqualVectorsWithDifferentCapacityAreNotEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-2"); 
    vector<int, 10> a;
    a.emplace_back(7);
    a.emplace_back(8);
    a.emplace_back(9);

    vector<int, 20> b;
    b.emplace_back(1);
    b.emplace_back(2);
    b.emplace_back(3);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST_F(vector_test, SubsetVectorWithSameCapacityIsNotEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-3"); 
    vector<int, 10> a;
    vector<int, 10> b;
    a.emplace_back(7);
    a.emplace_back(8);
    a.emplace_back(9);

    b.emplace_back(7);
    b.emplace_back(8);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST_F(vector_test, SubsetVectorWithDifferentCapacityIsNotEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-7"); 
    vector<int, 10> a;
    a.emplace_back(11);
    a.emplace_back(12);
    a.emplace_back(13);

    vector<int, 20> b;
    b.emplace_back(11);
    b.emplace_back(12);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST_F(vector_test, PartiallyEqualVectorsWithSameCapacityAreNotEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-6"); 
    vector<int, 10> a;
    vector<int, 10> b;

    a.emplace_back(14);
    a.emplace_back(15);
    a.emplace_back(16);
    b.emplace_back(14);
    b.emplace_back(15);
    b.emplace_back(666);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST_F(vector_test, PartiallyEqualVectorsWithDifferentCapacityAreNotEqual)
{
    ::testing::Test::RecordProperty("REQ", "ICO-4"); 
    vector<int, 10> a;
    a.emplace_back(17);
    a.emplace_back(18);
    a.emplace_back(19);

    vector<int, 20> b;
    b.emplace_back(17);
    b.emplace_back(18);
    b.emplace_back(999);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST_F(vector_test, FullVectorDestroysElementsInReverseOrder)
{
    ::testing::Test::RecordProperty("REQ", "ICO-3"); 
    static constexpr uint64_t VECTOR_CAPACITY = 35U;
    static constexpr uint64_t INDEX_END = VECTOR_CAPACITY - 1U;
    static constexpr uint64_t SOME_OFFSET = 9128U;

    {
        vector<CTorTest, VECTOR_CAPACITY> sut;

        for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
        {
            sut.emplace_back(i + SOME_OFFSET);
        }
    }

    ASSERT_THAT(stats.dTorOrder.size(), Eq(VECTOR_CAPACITY));
    for (size_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        EXPECT_THAT(stats.dTorOrder[i], Eq(INDEX_END - i + SOME_OFFSET));
    }
}

TEST_F(vector_test, PartiallyFullVectorDestroysElementsInReverseOrder)
{
    ::testing::Test::RecordProperty("REQ", "ICO-1"); 
    static constexpr uint64_t VECTOR_CAPACITY = 40U;
    static constexpr uint64_t VECTOR_SIZE = 20U;
    static constexpr uint64_t INDEX_END = VECTOR_SIZE - 1U;
    static constexpr uint64_t SOME_OFFSET = 1337U;

    {
        vector<CTorTest, VECTOR_CAPACITY> sut;

        for (uint64_t i = 0U; i < VECTOR_SIZE; ++i)
        {
            sut.emplace_back(i + SOME_OFFSET);
        }
    }

    ASSERT_THAT(stats.dTorOrder.size(), Eq(VECTOR_SIZE));
    for (size_t i = 0U; i < VECTOR_SIZE; ++i)
    {
        EXPECT_THAT(stats.dTorOrder[i], Eq(INDEX_END - i + SOME_OFFSET));
    }
}

TEST_F(vector_test, PopBackReturnsFalseOnEmptyVector)
{
    ::testing::Test::RecordProperty("REQ", "ICO-5"); 
    EXPECT_FALSE(sut.pop_back());
}

TEST_F(vector_test, PopBackReturnsTrueOnNonEmptyVector)
{
    ::testing::Test::RecordProperty("REQ", "ICO-6"); 
    sut.emplace_back(123U);
    EXPECT_TRUE(sut.pop_back());
}

TEST_F(vector_test, PopBackReturnsTrueTillItsEmpty)
{
    ::testing::Test::RecordProperty("REQ", "ICO-4"); 
    static constexpr uint64_t VECTOR_SIZE = 5U;
    for (uint64_t i = 0U; i < VECTOR_SIZE; ++i)
    {
        sut.emplace_back(i);
    }

    for (uint64_t i = 0U; i < VECTOR_SIZE; ++i)
    {
        EXPECT_TRUE(sut.pop_back());
    }

    EXPECT_FALSE(sut.pop_back());
}

TEST_F(vector_test, ResizeFailsWhenCountIsGreaterThanCapacity)
{
    ::testing::Test::RecordProperty("REQ", "ICO-1"); 
    EXPECT_FALSE(sut.resize(sut.capacity() + 1U));
}

TEST_F(vector_test, ResizeWithTemplateValueFailsWhenCountIsGreaterThanCapacity)
{
    ::testing::Test::RecordProperty("REQ", "ICO-2");  
    EXPECT_FALSE(sut.resize(sut.capacity() + 1U, 12U));
}

TEST_F(vector_test, SizeIncreaseWithResizeAndDefaultCTorWorks)
{
    ::testing::Test::RecordProperty("REQ", "ICO-4"); 
    class DefaultCTor
    {
      public:
        DefaultCTor() = default;
        int m_a{1231};
    };
    vector<DefaultCTor, 10U> sut;

    EXPECT_TRUE(sut.resize(5U));
    ASSERT_THAT(sut.size(), Eq(5U));
    for (auto& e : sut)
    {
        EXPECT_THAT(e.m_a, Eq(1231));
    }
}

TEST_F(vector_test, SizeIncreaseWithResizeAndTemplateValueWorks)
{
    ::testing::Test::RecordProperty("REQ", "ICO-3");  
    EXPECT_TRUE(sut.resize(4U, 421337U));
    ASSERT_THAT(sut.size(), Eq(4U));
    for (auto& e : sut)
    {
        EXPECT_THAT(e, Eq(421337));
    }
}

TEST_F(vector_test, SizeDecreaseWithResizeAndDefaultCTorWorks)
{
    ::testing::Test::RecordProperty("REQ", "ICO-2"); 
    constexpr uint64_t CAPACITY{10};
    vector<CTorTest, CAPACITY> sut;
    for (uint64_t i = 0U; i < CAPACITY; ++i)
    {
        sut.emplace_back(i);
    }

    EXPECT_TRUE(sut.resize(7U));
    EXPECT_THAT(stats.dTor, Eq(3U));
    ASSERT_THAT(stats.dTorOrder.size(), Eq(3U));
    EXPECT_THAT(stats.dTorOrder[0], Eq(9));
    EXPECT_THAT(stats.dTorOrder[1], Eq(8));
    EXPECT_THAT(stats.dTorOrder[2], Eq(7));
}

TEST_F(vector_test, SizeDecreaseWithResizeAndTemplateValueWorks)
{
    ::testing::Test::RecordProperty("REQ", "ICO-1"); 
    constexpr uint64_t CAPACITY{10};
    vector<CTorTest, CAPACITY> sut;
    for (uint64_t i = 0U; i < CAPACITY; ++i)
    {
        sut.emplace_back(i + 10U);
    }

    EXPECT_TRUE(sut.resize(7U, 66807U));
    EXPECT_THAT(stats.dTor, Eq(3U));
    ASSERT_THAT(stats.dTorOrder.size(), Eq(3U));
    EXPECT_THAT(stats.dTorOrder[0], Eq(19));
    EXPECT_THAT(stats.dTorOrder[1], Eq(18));
    EXPECT_THAT(stats.dTorOrder[2], Eq(17));
}

TEST_F(vector_test, ResizeWithDefaultCTorChangesNothingIfSizeAlreadyFits)
{
     
    sut.emplace_back(5U);
    sut.emplace_back(6U);
    EXPECT_TRUE(sut.resize(2U));

    ASSERT_THAT(sut.size(), Eq(2U));
    EXPECT_THAT(sut[0], Eq(5U));
    EXPECT_THAT(sut[1], Eq(6U));
}

TEST_F(vector_test, ResizeWithTemplateValueChangesNothingIfSizeAlreadyFits)
{
     
    sut.emplace_back(7U);
    sut.emplace_back(9U);
    EXPECT_TRUE(sut.resize(2U, 421337U));

    ASSERT_THAT(sut.size(), Eq(2U));
    EXPECT_THAT(sut[0], Eq(7U));
    EXPECT_THAT(sut[1], Eq(9U));
}
// step 1

TEST_F(vector_test, EmplaceInEmptyVectorWorks)
{
    ::testing::Test::RecordProperty("REQ", "ICO-1"); 
    EXPECT_TRUE(sut.emplace(0U, 123U));
    ASSERT_THAT(sut.size(), Eq(1U));
    EXPECT_THAT(sut[0], Eq(123U));
}


TEST_F(vector_test, EmplaceAtFrontTillFullWorks)
{
    ::testing::Test::RecordProperty("REQ", "ICO-2");  
    for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        EXPECT_TRUE(sut.emplace(0U, i));
        ASSERT_THAT(sut.size(), Eq(i + 1U));

        for (uint64_t n = 0U; n < sut.size(); ++n)
        {
            EXPECT_THAT(sut[n], Eq(sut.size() - n - 1U));
        }
    }
}

//step 2

TEST_F(vector_test, EmplacingElementInTheMiddleCallsDTor)
{
    ::testing::Test::RecordProperty("REQ", "ICO-3"); 
    constexpr uint64_t CAPACITY_OF_VECTOR{10U};
    constexpr uint64_t EXPECTED_NUMBER_OF_CTOR_CALLS{CAPACITY_OF_VECTOR};
    constexpr uint64_t EMPLACE_POSITION{5U};
    {
        vector<CTorTest, CAPACITY_OF_VECTOR> sut;
        for (uint64_t i = 0U; i < CAPACITY_OF_VECTOR - 1U; ++i)
        {
            sut.emplace_back(1234U);
        }

        EXPECT_THAT(stats.customCTor, Eq(EXPECTED_NUMBER_OF_CTOR_CALLS - 1U));
        EXPECT_TRUE(sut.emplace(EMPLACE_POSITION, 42U));
        EXPECT_THAT(stats.customCTor, Eq(EXPECTED_NUMBER_OF_CTOR_CALLS));
        EXPECT_THAT(stats.moveCTor, Eq(1U));
        EXPECT_THAT(stats.moveAssignment, Eq(CAPACITY_OF_VECTOR - 1U - EMPLACE_POSITION - 1U));
        EXPECT_THAT(stats.dTor, Eq(1U));
    }
    // Last element in the vector is moved and not constructed, hence #moveCTor + #customCTor = #dTor
    EXPECT_THAT(stats.moveCTor, Eq(1U));
    EXPECT_THAT(stats.customCTor, Eq(EXPECTED_NUMBER_OF_CTOR_CALLS));
    EXPECT_THAT(stats.dTor, Eq(EXPECTED_NUMBER_OF_CTOR_CALLS + 1U));
}

TEST_F(vector_test, EmplaceInTheMiddleMovesElementsToTheRight)
{
    ::testing::Test::RecordProperty("REQ", "ICO-4"); 
    sut.emplace_back(0U);
    sut.emplace_back(1U);
    sut.emplace_back(2U);

    EXPECT_TRUE(sut.emplace(1U, 3U));

    ASSERT_THAT(sut.size(), Eq(4));

    EXPECT_THAT(sut[0], Eq(0U));
    EXPECT_THAT(sut[1], Eq(3U));
    EXPECT_THAT(sut[2], Eq(1U));
    EXPECT_THAT(sut[3], Eq(2U));
}

TEST_F(vector_test, EmplaceWhenFullReturnsFalse)
{
    ::testing::Test::RecordProperty("REQ", "ICO-5"); 
    for (uint64_t i = 0U; i < VECTOR_CAPACITY; ++i)
    {
        sut.emplace_back(i);
    }

    auto index = VECTOR_CAPACITY / 2;
    EXPECT_FALSE(sut.emplace(index, 5U));
    EXPECT_THAT(sut.size(), Eq(sut.capacity()));
}

TEST_F(vector_test, EmplaceWhenPositionExceedsCapacityReturnsFalse)
{
    ::testing::Test::RecordProperty("REQ", "ICO-6"); 
    EXPECT_FALSE(sut.emplace(sut.capacity() + 10U, 5U));
    EXPECT_THAT(sut.size(), Eq(0));
}


TEST_F(vector_test, EmplaceAtEndWorks)
{
    ::testing::Test::RecordProperty("REQ", "CIC-5"); 
    sut.emplace_back(0U);
    sut.emplace_back(1U);

    EXPECT_TRUE(sut.emplace(sut.size(), 3U));
    ASSERT_THAT(sut.size(), Eq(3U));
    EXPECT_THAT(sut[0], Eq(0U));
    EXPECT_THAT(sut[1], Eq(1U));
    EXPECT_THAT(sut[2], Eq(3U));
}
// step 3

TEST_F(vector_test, EmplaceAtPositionAfterEndBeforeCapacityExceedsFails)
{
    ::testing::Test::RecordProperty("REQ", "ICO-7"); 
    sut.emplace_back(0U);
    sut.emplace_back(1U);

    constexpr uint64_t EXPECTED_SIZE{2};
    ASSERT_THAT(sut.size(), EXPECTED_SIZE);
    EXPECT_FALSE(sut.emplace(EXPECTED_SIZE + 1, 3U));
    ASSERT_THAT(sut.size(), EXPECTED_SIZE);
}
  
} // namespace
