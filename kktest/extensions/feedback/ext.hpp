#pragma once

#include "common/interproc/pipe.hpp"
#include "kktest/core/extension.hpp"
#include "kktest/extensions/feedback/test_logger.hpp"

namespace kktest::feedback {

class FeedbackExtension : public Extension {
  public:
    int getReturnCode();

    void registerCommandLineArgs(mcga::cli::Parser& parser) override;

    void init(HooksManager& api) override;

  private:
    void initLogging(HooksManager& api);

    void initFileStream(HooksManager& api, const std::string& fileName);

    bool failedAnyNonOptionalTests = false;
    std::unique_ptr<TestLogger> logger = nullptr;
    std::unique_ptr<interproc::PipeWriter> fileWriter = nullptr;

    mcga::cli::Flag quietFlag;
    mcga::cli::Argument fileNameArgument;
};

}  // namespace kktest::feedback
