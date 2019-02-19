#include "extensions/death/include/kktest_ext/death_impl/check_death.hpp"

#include "common/interproc/src/worker_subprocess.hpp"
#include "common/utils/src/process_time.hpp"
#include "core/src/executor.hpp"
#include "core/src/export.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;

namespace kktest {
namespace death {

KKTEST_EXPORT DeathStatus checkDeath(const function<void()>& func,
                                     double timeTicksLimit) {
    WorkerSubprocess proc(
        timeTicksLimit * Executor::getTimeTickLengthMs(),
        [func](PipeWriter* writer) {
            func();
            writer->sendMessage(1);
        });

    while (!proc.isFinished()) {
        sleepForDuration(Duration::fromMs(5));
    }
    if (proc.getNextMessage(32).isInvalid()) {
        // The exit was ok.
        return DeathStatus(proc.getReturnCode(),
                           proc.getSignal(),
                           proc.getOutput());
    }
    return DeathStatus(-1, 0, proc.getOutput());
}

}
}
