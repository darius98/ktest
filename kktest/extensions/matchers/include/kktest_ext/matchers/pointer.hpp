#pragma once

#include <kktest_ext/matchers/detail/pointer.hpp>

namespace mcga::test::matchers {

constexpr detail::IsNullptrMatcher isNull;

constexpr detail::IsNotNullptrMatcher isNotNull;

}  // namespace mcga::test::matchers
