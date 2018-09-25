#include "group.hpp"

using namespace std;

namespace runtime_testing {

Group::Group(const std::string &description): description(description) {}

Group::~Group() {
    for (Test* test: this->tests) {
        delete test;
    }
    for (Group* group: this->subGroups) {
        delete group;
    }
}

int Group::generateTestReport(ostream& report,
                              const string& currentGroupFullName) {
    for (Test* test: this->tests) {
        test->report(report, currentGroupFullName);
    }
    for (Group* group: this->subGroups) {
        group->generateTestReport(
            report,
            currentGroupFullName + " > " + group->description
        );
    }
    return this->numFailedTests;
}

}