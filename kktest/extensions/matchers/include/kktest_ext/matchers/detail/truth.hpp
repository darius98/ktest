#pragma once

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

class IsTrueMatcher: public StatelessMatcher {
 public:
    bool matches(const bool& object) const {
        return object;
    }

    void describe(Description* description) const {
        (*description) << "true";
    }

    void describeFailure(Description* description) const {
        (*description) << "false";
    }
};

class IsFalseMatcher: public StatelessMatcher {
 public:
    bool matches(const bool& object) const {
        return !object;
    }

    void describe(Description* description) const {
        (*description) << "false";
    }

    void describeFailure(Description* description) const {
        (*description) << "true";
    }
};

}
