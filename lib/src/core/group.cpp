#include <cstring>

#include "group.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

int Group::globalGroupIndex = 0;

Group::Group(string _description, string _file, int _line, Group* _parentGroup):
        description(move(_description)),
        file(move(_file)),
        line(_line),
        parentGroup(_parentGroup),
        index(++globalGroupIndex) {}

Group::~Group() {
    for (Test* test: tests) {
        delete test;
    }
    for (Group* subGroup: subGroups) {
        delete subGroup;
    }
}

int Group::getNumFailedTests() const {
    int numFailedTests = 0;
    for (Test* test: tests) {
        numFailedTests += test->isFailed();
    }
    for (Group* subGroup: subGroups) {
        numFailedTests += subGroup->getNumFailedTests();
    }
    return numFailedTests;
}

int Group::getNumTests() const {
    int numTests = tests.size();
    for (Group* subGroup: subGroups) {
        numTests += subGroup->getNumTests();
    }
    return numTests;
}

bool Group::isGlobalScope() const {
    return parentGroup == nullptr;
}

Group* Group::addSubGroup(string description, string file, int line) {
    Group* subGroup = new Group(move(description), move(file), line, this);
    subGroups.push_back(subGroup);
    return subGroup;
}

Test* Group::addTest(string description, string file, int line) {
    Test* test = new Test(move(description), move(file), line, this);
    tests.push_back(test);
    return test;
}

void Group::addSetUp(Executable func) {
    if (hasSetUp) {
        throw runtime_error("Group '" + getFullDescription() +
                            "' already has a setUp!");
    }
    hasSetUp = true;
    setUpFunc = func;
}

void Group::setUp() const {
    if (hasSetUp) {
        setUpFunc();
    }
}

void Group::addTearDown(Executable func) {
    if (hasTearDown) {
        throw runtime_error("Group '" + getFullDescription() +
                            "' already has a tearDown!");
    }
    hasTearDown = true;
    tearDownFunc = func;
}

void Group::tearDown() const {
    if (hasTearDown) {
        tearDownFunc();
    }
}

string Group::getFullDescription() const {
    string fullDescription = description;
    if (!parentGroup->isGlobalScope()) {
        fullDescription = parentGroup->getFullDescription()
                          + "::"
                          + fullDescription;
    }
    return fullDescription;
}

int Group::getIndex() const {
    return index;
}

void Group::writeBytes(BytesConsumer& consumer) const {
    if (isGlobalScope()) {
        return;
    }
    consumer
        << (parentGroup->isGlobalScope() ? 0 : parentGroup->getIndex())
        << index
        << line
        << file.size()
        << file
        << description.size()
        << description;
}

JSON Group::toJSON() const {
    JSON report;
    if (!isGlobalScope()) {
        report = {
            {"line", line},
            {"file", file},
            {"description", description}
        };
    } else {
        report = {
            {"numTests", getNumTests()},
            {"numFailedTests", getNumFailedTests()}
        };
    }

    if (!tests.empty()) {
        report["tests"] = vector<JSON>();
        for (const auto& test: tests) {
            report["tests"].push_back(test->toJSON());
        }
    }
    if (!subGroups.empty()) {
        report["subGroups"] = vector<JSON>();
        for (const auto& subGroup: subGroups) {
            report["subGroups"].push_back(subGroup->toJSON());
        }
    }
    return report;
}

Group* Group::getParentGroup() const {
    return parentGroup;
}

}