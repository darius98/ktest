#pragma once

#include "common/interproc/subprocess.hpp"
#include "common/interproc/pipe.hpp"
#include "common/utils/time.hpp"

namespace kktest::interproc {

class WorkerSubprocess: public Subprocess {
 public:
    typedef const std::function<void(PipeWriter*)>& Work;

    WorkerSubprocess(utils::Duration timeLimit, Work run);

    WorkerSubprocess(WorkerSubprocess&& other) noexcept;

    WorkerSubprocess(const WorkerSubprocess& other) = delete;

    ~WorkerSubprocess() override;

    bool isFinished() override;

    KillResult kill() override;

    bool isExited() override;

    int getReturnCode() override;

    bool isSignaled() override;

    int getSignal() override;

    FinishStatus getFinishStatus() override;

    Message getNextMessage(int maxConsecutiveFailedReadAttempts = -1);

 private:
    Subprocess* subprocess;
    PipeReader* pipeReader;
    utils::Stopwatch stopwatch;
};

}