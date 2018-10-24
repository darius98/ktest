#ifndef KKTEST_TESTING_EXECUTOR_H_
#define KKTEST_TESTING_EXECUTOR_H_

#include <set>

#include "executable.hpp"
#include "group.hpp"
#include "test.hpp"


namespace kktest {

class Executor {
public:
    typedef std::function<void(Test*)> TestHook;
    typedef std::function<void(Group*)> GroupHook;

    explicit Executor(int _testIndexToRun);

    virtual bool isDuringTest() const = 0;

    virtual void checkIsInactive(const std::string& methodName) const = 0;

    virtual void finalize() = 0;

private:
    virtual void execute(const std::vector<Group*>& groups,
                         Test* test,
                         Executable func) = 0;

public:
    void executeTest(const std::vector<Group*>& groups,
                     Test* test,
                     Executable func);

    void addBeforeTestHook(TestHook hook);
    void addAfterTestHook(TestHook hook);
    void addBeforeGroupHook(GroupHook hook);
    void addAfterGroupHook(GroupHook hook);

    void afterTest(Test* test) const;
    void beforeTest(Test* test) const;
    void beforeGroup(Group* group) const;
    void afterGroup(Group* group) const;

private:
    int testIndexToRun;

    std::vector<TestHook> beforeTestHooks;
    std::vector<TestHook> afterTestHooks;
    std::vector<GroupHook> beforeGroupHooks;
    std::vector<GroupHook> afterGroupHooks;
};

}

#endif
