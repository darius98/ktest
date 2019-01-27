#define VERSION "1.0.0"

#include <iostream>

#include <kktest_impl/main.hpp>
#include <kktest_impl/signature.hpp>

#include <kktest_ext/feedback.hpp>

using namespace kktest;
using namespace cppli;
using namespace std;

int main(int argc, char** argv) {
    Cppli cppli("Test binary. Generated by The KKTest Framework.");
    cppli.addHelpFlag();

    kktest::feedback::FeedbackExtension feedbackExtension;
    feedbackExtension.registerCommandLineArgs(cppli);

    InternalArgs internalArgs = registerInternalFlags(cppli, VERSION);

    cppli.interpret(argc, argv);

    return kktest::main({&feedbackExtension}, internalArgs);
}