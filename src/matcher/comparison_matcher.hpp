#ifndef EQUALITY_TRUTH_MATCHER_H_
#define EQUALITY_TRUTH_MATCHER_H_

#include <functional>

#include "matcher.hpp"

namespace matcher {

template<class T>
class ComparisonMatcher : public Matcher<T> {
public:
    typedef std::function<bool(const T&, const T&)> Comparator;

    ComparisonMatcher(const T& target,
                      Comparator comparator,
                      const char* expectation) :
            target(target), comparator(comparator), expectation(expectation) {}

    bool matches(const T& object) const override {
        return this->comparator(object, this->target);
    }

    void describe(const T& object,
                  Description* description) const override {
        description->append(
            this->expectation,
            "'",
            this->target,
            "', got '",
            object,
            "'"
        );
    }

private:
    Comparator comparator;
    const T& target;
    const char* expectation;
};

} // namespace matcher

#endif