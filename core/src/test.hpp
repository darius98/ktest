#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include "core/include/kktest.hpp"
#include "core/src/group.hpp"

namespace kktest {

class Test : public TestConfig {
 public:
    Test(TestConfig&& _config, Group* _group, int _index);

    Test(Test&& other) noexcept;
    Test& operator=(Test&& other) noexcept;

    Test(const Test& other) = delete;
    Test& operator=(const Test& other) = delete;

    Group* getGroup() const;

    int getIndex() const;

 private:
    Group* group;
    int index;
};

class ExecutedTest : public Test {
 public:
    struct Info {
        double timeTicks;
        bool passed;
        std::string failure;

        void fail(const std::string& _failure);
    };

    // When we create a test run, we no longer need the test.
    // So we always move it.
    ExecutedTest(Test&& test, std::string failure);
    ExecutedTest(Test&& test, Info&& info);

    bool isPassed() const;

    std::string getFailure() const;

 private:
    Info info;
};

}

#endif
