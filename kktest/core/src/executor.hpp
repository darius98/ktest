#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include "kktest/core/src/executed_test.hpp"

namespace kktest {

class Executor {
 public:
    typedef std::function<void(const ExecutedTest&)> OnTestFinished;

    static double getTimeTickLengthMs();

    explicit Executor(OnTestFinished onTestFinished);

    virtual ~Executor() = default;

    void checkIsInactive(const std::string& methodName) const;

    virtual void execute(Test test);

    virtual void finalize();

    ExecutedTest::Info run(const Test& test);

 private:
    void runJob(const Executable& job,
                ExecutedTest::Info* execution,
                const std::string& where);

 protected:
    OnTestFinished onTestFinished;

 private:
    enum { ACTIVE, INACTIVE } state = INACTIVE;
};

}

#endif