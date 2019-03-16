#include "kktest/extensions/feedback/ext.hpp"

#include <iostream>
#include <memory>

using namespace cppli;
using namespace kktest::interproc;
using namespace std;

template<>
Message::BytesConsumer& Message::BytesConsumer::add(
        const vector<ExecutedTest::Info>& obj) {
    add(obj.size());
    for (const auto& info : obj) {
        add(info.timeTicks, info.passed, info.failure);
    }
    return *this;
}

namespace kktest::feedback {

enum PipeMessageType : uint8_t {
    TEST_STARTED = 0,
    TEST_DONE = 1,
    GROUP = 2,
    DONE = 3,
    ERROR = 4
};

void FeedbackExtension::registerCommandLineArgs(Parser& parser) {
    quietFlag = parser.addFlag(
        FlagSpec("quiet")
        .setHelpGroup("Feedback")
        .setDescription("Disable STDOUT logging for this test run")
        .setShortName("q"));
    pipeNameArgument = parser.addArgument(
        ArgumentSpec("pipe-to")
        .setHelpGroup("Feedback")
        .setDescription("A file or fifo with write access for piping the test "
                        "results as they become available."));
}

void FeedbackExtension::init(HooksManager& api) {
    if (!quietFlag.get()) {
        initLogging(api);
    }
    if (!pipeNameArgument.get().empty()) {
        initPipe(api, pipeNameArgument.get());
    }
}

void FeedbackExtension::initLogging(HooksManager& api) {
    logger = make_unique<TestLogger>(cout);

    api.addHook<HooksManager::AFTER_TEST>([this](const ExecutedTest& test) {
        logger->addTest(test);
    });

    api.addHook<HooksManager::BEFORE_DESTROY>([this]() {
        logger->printFinalInformation();
    });

    api.addHook<HooksManager::BEFORE_FORCE_DESTROY>([this](const auto& err) {
        logger->printFatalError(err.what());
    });
}

void FeedbackExtension::initPipe(HooksManager& api, const string& pipeName) {
    pipe = unique_ptr<PipeWriter>(openNamedPipeForWriting(pipeName));

    api.addHook<HooksManager::BEFORE_GROUP>([this](GroupPtr group) {
        pipe->sendMessage(PipeMessageType::GROUP,
                          group->getParentGroup()->getId(),
                          group->getId(),
                          group->getDescription());
    });

    api.addHook<HooksManager::BEFORE_TEST>([this](const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_STARTED,
                          test.getId(),
                          test.getGroup()->getId(),
                          test.getDescription(),
                          test.isOptional(),
                          test.getNumAttempts(),
                          test.getNumRequiredPassedAttempts());
    });

    api.addHook<HooksManager::AFTER_TEST>([this](const ExecutedTest& test) {
        pipe->sendMessage(PipeMessageType::TEST_DONE,
                          test.getId(),
                          test.getExecutions());
    });

    api.addHook<HooksManager::BEFORE_DESTROY>([this]() {
        pipe->sendMessage(PipeMessageType::DONE);
    });

    api.addHook<HooksManager::BEFORE_FORCE_DESTROY>([this](const auto& err) {
        pipe->sendMessage(PipeMessageType::ERROR, string(err.what()));
    });
}

}
