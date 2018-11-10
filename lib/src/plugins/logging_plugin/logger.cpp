#include <cstdio>
#include <iostream>
#include <unistd.h>

#include "logger.hpp"

using namespace std;

namespace kktest {

Logger::Logger(ostream& _stream): stream(_stream) {}

void Logger::enqueueTestForLogging(Test* test) {
    testsQueue.insert(test);
    while (!testsQueue.empty() && (*testsQueue.begin())->getIndex() == testsLogged + 1) {
        logTest(*testsQueue.begin());
        testsQueue.erase(testsQueue.begin());
        testsLogged += 1;
    }
}

void Logger::logFinalInformation(int passedTests, int failedTests, int failedOptionalTests) {
    stream << "\n";
    stream << "Tests passed: ";
    modifyOutput(32);
    stream << passedTests;
    modifyOutput(0);
    stream << "\nTests failed: ";
    modifyOutput((failedTests == failedOptionalTests ? (failedTests == 0 ? 32 : 33) : 31));
    stream << failedTests;
    modifyOutput(0);
    if (failedOptionalTests) {
        stream << " (";
        modifyOutput(33);
        stream << failedOptionalTests;
        modifyOutput(0);
        stream << " " << (failedOptionalTests == 1 ? "was" : "were") << " optional)";
    }
    stream << "\n";
}

bool Logger::isInTerminal() const {
    return stream.rdbuf() == cout.rdbuf() && isatty(fileno(stdout)) != 0;
}

void Logger::modifyOutput(const int& code) {
    if (!isInTerminal()) {
        return;
    }
    string output = "\x1b[" + to_string(code) + "m";
    stream << output.c_str();
}

void Logger::logTest(Test *test) {
    modifyOutput(90);
    stream << test->getDescriptionPrefix();
    modifyOutput(0);
    stream << test->getConfig().description;
    stream << ": ";
    if (test->isFailed()) {
        modifyOutput(31);
        stream << "FAILED\n";
        modifyOutput(0);
        string failureMessage = test->getFailureMessage();
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        while ((pos = failureMessage.find('\n', pos)) != string::npos) {
            failureMessage.replace(pos, 1, "\n\t");
            pos += 2;
        }
        stream << "\t" << failureMessage << "\n";
    } else {
        modifyOutput(32);
        stream << "PASSED\n";
        modifyOutput(0);
    }
    stream.flush();
}

bool Logger::AscendingByTestIndex::operator()(Test* a, Test* b) const {
    return a->getIndex() < b->getIndex();
}

}
