#include "kktest/core/include/kktest.hpp"

#include "kktest/core/src/driver.hpp"

using namespace std;

namespace kktest {

TestConfig::TestConfig(string description): description(move(description)) {}

TestConfig::TestConfig(const char* description): description(description) {}

TestConfig& TestConfig::setDescription(string description) {
    this->description = move(description);
    return *this;
}

TestConfig& TestConfig::setOptional(bool optional) {
    this->optional = optional;
    return *this;
}

TestConfig& TestConfig::setTimeTicksLimit(double timeTicksLimit) {
    this->timeTicksLimit = timeTicksLimit;
    return *this;
}

TestConfig& TestConfig::setAttempts(size_t attempts) {
    this->attempts = attempts;
    return *this;
}

TestConfig& TestConfig::setRequiredPassedAttempts(
        size_t requiredPassedAttempts) {
    this->requiredPassedAttempts = requiredPassedAttempts;
    return *this;
}

GroupConfig::GroupConfig(string description): description(move(description)) {}

GroupConfig::GroupConfig(const char* description): description(description) {}

GroupConfig& GroupConfig::setDescription(string description) {
    this->description = move(description);
    return *this;
}

GroupConfig& GroupConfig::setOptional(bool optional) {
    this->optional = optional;
    return *this;
}

TestCase::TestCase(void (*exec)()): exec(exec), name("") {}

TestCase::TestCase(Executable exec): exec(move(exec)) {}

TestCase::TestCase(Executable exec, std::string name):
        exec(move(exec)), name(move(name)) {}

void TestCase::run() {
    exec();
}

void test(TestConfig config, Executable func) {
    Driver::getInstance()->addTest(move(config), move(func));
}

void group(GroupConfig config, const Executable& func) {
    Driver::getInstance()->addGroup(move(config), func);
}

void setUp(Executable func) {
    Driver::getInstance()->addSetUp(move(func));
}

void tearDown(Executable func) {
    Driver::getInstance()->addTearDown(move(func));
}

void expect(bool expr, const string& message) {
    if (!expr) {
        fail(message);
    }
}

void fail(const string& message) {
    throw ExpectationFailed(message);
}

}