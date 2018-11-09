#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include <core/matcher.hpp>

#include "matcher_plugin.hpp"

AddArgument(int, flagEnableMatchers)
    .ArgumentType("0|1 ")
    .Name("enable_matchers")
    .Short("m")
    .Description("Enable usage of the matcher library")
    .DefaultValue(1)
    .ImplicitValue(1);


namespace kktest {

MatcherPlugin::MatcherPlugin() {
    Matcher::testingStarted = true;
}

bool MatcherPlugin::isEnabled() const {
    return flagEnableMatchers != 0;
}

void MatcherPlugin::install() {
    Matcher::matcherPluginEnabled = true;
    TestingDriver::addBeforeTestHook([](Test*) {
        Matcher::duringTest = true;
    });

    TestingDriver::addAfterTestHook([](Test*) {
        Matcher::duringTest = false;
        Matcher::cleanupMatchersCreatedDuringTests();
    });
}

}
