#include <fstream>

#include <EasyFlags.hpp>

#include <core/driver.hpp>

using namespace autojson;
using namespace std;

AddArgument(string, argumentReportFileName)
    .Name("report")
    .Short("r")
    .ArgumentType("FILE ")
    .Description("Generate a JSON test report at the end of running the tests")
    .DefaultValue("")
    .ImplicitValue("./report.json");

namespace kktest {

class ReportPlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override {
        return !argumentReportFileName.empty();
    }

    void install() override {
        report["numTests"] = 0;
        report["numFailedTests"] = 0;

        TestingDriver::addBeforeGroupHook([this](Group* group) {
            groupJSONs[group->getIndex()] = map<string, JSON>{
                    {"line", group->getConfig().line},
                    {"file", group->getConfig().file},
                    {"description", group->getConfig().description},
                    {"index", group->getIndex()}
            };
        });

        TestingDriver::addAfterGroupHook([this](Group* group) {
            JSON groupJSON = groupJSONs[group->getIndex()];
            JSON& parentJSON = group->isTopLevel() ? report : groupJSONs[group->getParentGroupIndex()];
            if (!parentJSON.exists("subGroups")) {
                parentJSON["subGroups"] = vector<JSON>();
            }
            parentJSON["subGroups"].push_back(groupJSON);
        });

        TestingDriver::addAfterTestHook([this](Test* test) {
            JSON testJSON = {
                    {"description", test->getConfig().description},
                    {"optional", test->getConfig().optional},
                    {"file", test->getConfig().file},
                    {"line", test->getConfig().line},
                    {"executed", test->isExecuted()},
                    {"index", test->getIndex()}
            };
            if (test->isExecuted()) {
                report["numTests"] = (int)report["numTests"] + 1;
                testJSON["passed"] = test->isPassed();
                if (!test->isPassed()) {
                    report["numFailedTests"] = (int)report["numFailedTests"] + 1;
                    testJSON["failureMessage"] = test->getFailureMessage();
                }
            }

            JSON& parentJSON = test->isTopLevel() ? report : groupJSONs[test->getGroupIndex()];
            if (!parentJSON.exists("tests")) {
                parentJSON["tests"] = vector<JSON>();
            }
            parentJSON["tests"].push_back(testJSON);
        });

        TestingDriver::addBeforeDestroyHook([this]() {
            ofstream reportFile(argumentReportFileName);
            reportFile << report.stringify(0);
            reportFile.close();
        });
    }

private:
    autojson::JSON report;
    std::map<int, autojson::JSON> groupJSONs;
};

ReportPlugin reportPlugin("kktest");

}