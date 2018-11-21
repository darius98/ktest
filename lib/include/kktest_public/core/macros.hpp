#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MACROS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MACROS_H_

#include "./definers.hpp"
#include "./filename.hpp"

#define KKTEST_FILE_NAME_ (kktest::trimFileName(__FILE__))

#define kkTest kktest::TestDefiner(KKTEST_FILE_NAME_, __LINE__)

#define kkGroup kktest::GroupDefiner(KKTEST_FILE_NAME_, __LINE__)

#define kkSetUp kktest::SetUpDefiner(KKTEST_FILE_NAME_, __LINE__)

#define kkTearDown kktest::TearDownDefiner(KKTEST_FILE_NAME_, __LINE__)

#define kkExpect(...) kktest::ExpectDefiner(KKTEST_FILE_NAME_, __LINE__)                           \
                                         (__VA_ARGS__, #__VA_ARGS__ " is false")

#define kkFail(...) kktest::ExpectDefiner(KKTEST_FILE_NAME_, __LINE__)(false, __VA_ARGS__)

#define KKTESTCASE_DEF_AUX_1_(x, y) x##_##y
#define KKTESTCASE_DEF_AUX_2_(x, y) KKTESTCASE_DEF_AUX_1_(x, y)
#define KKTESTCASE_DEF_AUX_3_(COUNTER)                                                             \
            KKTESTCASE_DEF_AUX_2_(kkTestCaseInstance, COUNTER)();                                  \
            static kktest::TestCaseRegistryKeeper KKTESTCASE_DEF_AUX_2_(kkTestCaseDefiner, COUNTER)\
                    (KKTESTCASE_DEF_AUX_2_(kkTestCaseInstance, COUNTER));                          \
            void KKTESTCASE_DEF_AUX_2_(kkTestCaseInstance, COUNTER)

#define kkTestCase KKTESTCASE_DEF_AUX_3_(__COUNTER__)

#endif
