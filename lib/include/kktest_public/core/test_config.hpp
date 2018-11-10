#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_TEST_CONFIG_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_TEST_CONFIG_H_

#include <string>


namespace kktest {

struct TestConfig {
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1000.0;
};

}

#define testConfig(...) ([&]{kktest::TestConfig $;__VA_ARGS__; return $;}())

#endif
