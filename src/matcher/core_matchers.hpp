#ifndef KKTEST_MATCHER_CORE_MATCHERS_H_
#define KKTEST_MATCHER_CORE_MATCHERS_H_

#include <set>

#include "comparison_matchers.hpp"
#include "composite_matchers.hpp"
#include "functional_matchers.hpp"
#include "iterable_matchers.hpp"
#include "numeric_matchers.hpp"
#include "pointer_matchers.hpp"
#include "string_matchers.hpp"
#include "truth_matcher.hpp"


namespace kktest {

extern IsTrueMatcher* isTrue;

extern IsFalseMatcher* isFalse;

template<class T>
ComparisonMatcher<T>* isEqualTo(const T& object) {
    return new ComparisonMatcher<T>(object, std::equal_to<T>(), "");
}

template<class T>
ComparisonMatcher<T>* isLessThan(const T &object) {
    return new ComparisonMatcher<T>(object, std::less<T>(), "< ");
}

template<class T>
ComparisonMatcher<T>* isLessThanEqual(const T &object) {
    return new ComparisonMatcher<T>(object, std::less_equal<T>(), "<= ");
}

template<class T>
ComparisonMatcher<T>* isGreaterThan(const T &object) {
    return new ComparisonMatcher<T>(object, std::greater<T>(), "> ");
}

template<class T>
ComparisonMatcher<T>* isGreaterThanEqual(const T &object) {
    return new ComparisonMatcher<T>(object, std::greater_equal<T>(), ">= ");
}

template<class T>
IdentityMatcher<T>* isIdenticalTo(const T& object) {
    return new IdentityMatcher<T>(object);
}

extern IsEmptyMatcher* isEmpty;

extern IsNotEmptyMatcher* isNotEmpty;

template<class M, IS_MATCHER(M)>
IterableSizeMatcher<M>* hasSize(M* sizeMatcher) {
    return new IterableSizeMatcher<M>(sizeMatcher);
}

template<class T>
IterableSizeMatcher<ComparisonMatcher<T>>* hasSize(const T& object) {
    return new IterableSizeMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class M, IS_MATCHER(M)>
IterableEachMatcher<M>* eachElement(M* elementMatcher) {
    return new IterableEachMatcher<M>(elementMatcher);
}

template<class T>
IterableEachMatcher<ComparisonMatcher<T>>* eachElement(const T &object) {
    return new IterableEachMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class M, IS_MATCHER(M)>
IterableAnyMatcher<M>* anyElement(M *elementMatcher) {
    return new IterableAnyMatcher<M>(elementMatcher);
}

template<class T>
IterableAnyMatcher<ComparisonMatcher<T>>* anyElement(const T &object) {
    return new IterableAnyMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
AndMatcher<M1, M2>* both(M1* m1, M2* m2) {
    return new AndMatcher<M1, M2>(m1, m2);
}

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
OrMatcher<M1, M2>* either(M1* m1, M2* m2) {
    return new OrMatcher<M1, M2>(m1, m2);
}

template<class M, IS_MATCHER(M)>
NotMatcher<M>* isNot(M* matcher) {
    return new NotMatcher<M>(matcher);
}

extern IsNullptrMatcher* isNull;

extern IsNotNullptrMatcher* isNotNull;

extern ThrowsAnythingMatcher* throws;

template<class E>
ThrowsSpecificMatcher<E>* throwsA() {
    return new ThrowsSpecificMatcher<E>();
}

extern IsPositiveMatcher* isPositive;

extern IsNegativeMatcher* isNegative;

extern IsEvenMatcher* isEven;

extern IsOddMatcher* isOdd;

extern IsZeroMatcher* isZero;

template<class T>
IsAlmostEqualMatcher<T>* isAlmostEqualTo(const T& target,
                                         const double& eps=0.000001) {
    return new IsAlmostEqualMatcher<T>(target, eps);
}

extern CharInStringMatcher* isLetter;

extern CharInStringMatcher* isDigit;

extern CharInStringMatcher* isLowercaseLetter;

extern CharInStringMatcher* isUppercaseLetter;

extern CharInStringMatcher* isBinaryDigit;

extern CharInStringMatcher* isOctDigit;

extern CharInStringMatcher* isHexDigit;

extern CharInStringMatcher* isWhitespace;

IsSubstringMatcher* isSubstringOf(const std::string& s);

} // namespace kktest

#endif
