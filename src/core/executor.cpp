#include "executor.hpp"

#include <chrono>
#include <stdexcept>
#include <thread>

#include "time_tick.hpp"

using std::exception;
using std::hash;
using std::move;
using std::runtime_error;
using std::string;
using std::thread;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;
namespace this_thread = std::this_thread;

namespace mcga::test {

class ExpectationFailed : public runtime_error {
    using runtime_error::runtime_error;
};

Executor::Executor(HooksManager* hooks): hooks(hooks) {
}

bool Executor::isActive() const {
    return state != INACTIVE;
}

string Executor::stateAsString() const {
    switch (state) {
        case INSIDE_TEST: return "test";
        case INSIDE_SET_UP: return "setUp";
        case INSIDE_TEAR_DOWN: return "tearDown";
        default: return "inactive";
    }
}

void Executor::finalize() {
}

void Executor::addFailure(const string& failure) {
    // We only kill the thread on failure if we are in the main testing thread
    // and we know we catch this exception.
    if (hash<thread::id>()(this_thread::get_id()) == currentExecutionThreadId) {
        throw ExpectationFailed(failure);
    }

    // If the user starts his own threads that entertain failures, it is his
    // responsibility to make sure his threads die on failure (we have no
    // control)
    std::lock_guard guard(currentExecutionFailureMutex);
    if (!currentExecutionIsFailed) {
        currentExecutionIsFailed = true;
        currentExecutionFailureMessage = failure;
    }
}

void Executor::execute(Test test) {
    for (size_t i = 0; i < test.getNumAttempts(); ++i) {
        onTestExecutionStart(test);
        test.addExecution(run(test));
        onTestExecutionFinish(test);
    }
}

Executor::Type Executor::getType() const {
    return SMOOTH;
}

Test::ExecutionInfo Executor::run(const Test& test) {
    currentTestId = test.getId();
    state = INSIDE_SET_UP;
    Test::ExecutionInfo info;
    auto startTime = high_resolution_clock::now();
    vector<GroupPtr> testGroupStack = test.getGroupStack();
    vector<GroupPtr>::iterator it;
    // Execute setUp()-s, in the order of the group stack.
    for (it = testGroupStack.begin(); it != testGroupStack.end(); ++it) {
        runJob([it] { (*it)->setUp(); },
               &info,
               "setUp of group \"" + (*it)->getDescription() + "\"");
        if (!info.passed) {
            // If a setUp() fails, do not execute the rest.
            break;
        }
    }
    state = INSIDE_TEST;
    if (info.passed) {
        // Only run the test if all setUp()-s passed without exception.
        runJob([&test] { test.run(); }, &info, "test");
        --it;
    }
    state = INSIDE_TEAR_DOWN;
    // Execute tearDown()-s in reverse order, from where setUp()-s stopped.
    for (; it + 1 != testGroupStack.begin(); --it) {
        runJob([it] { (*it)->tearDown(); },
               &info,
               "tearDown of group \"" + (*it)->getDescription() + "\"");
    }
    auto endTime = high_resolution_clock::now();
    auto nanosecondsElapsed
      = duration_cast<nanoseconds>(endTime - startTime).count();
    info.timeTicks = 1.0 * nanosecondsElapsed / GetTimeTickLength().count();
    if (info.timeTicks > test.getTimeTicksLimit()) {
        info.fail("Execution timed out.");
    }
    state = INACTIVE;
    return info;
}

void Executor::emitWarning(const string& message, std::size_t groupId) {
    if (isActive()) {
        onWarning(Warning(message, groupId, currentTestId));
    }
}

void Executor::runJob(const Executable& job,
                      Test::ExecutionInfo* execution,
                      const string& where) {
    currentExecutionThreadId = hash<thread::id>()(this_thread::get_id());
    currentExecutionIsFailed = false;
    currentExecutionFailureMessage = "";

    try {
        job();
    } catch (const ExpectationFailed& failure) {
        execution->fail(failure.what());
    } catch (const exception& e) {
        execution->fail("Uncaught exception in " + where + ": " + e.what());
    } catch (...) {
        execution->fail("Uncaught non-exception type in " + where + "\".");
    }

    std::lock_guard guard(currentExecutionFailureMutex);
    if (currentExecutionIsFailed) {
        execution->fail(currentExecutionFailureMessage);
    }
}

void Executor::onWarning(const Warning& warning) {
    hooks->runHooks<HooksManager::ON_WARNING>(warning);
}

void Executor::onTestExecutionStart(const Test& test) {
    hooks->runHooks<HooksManager::ON_TEST_EXECUTION_START>(test);
}

void Executor::onTestExecutionFinish(const Test& test) {
    hooks->runHooks<HooksManager::ON_TEST_EXECUTION_FINISH>(test);
}

}  // namespace mcga::test
