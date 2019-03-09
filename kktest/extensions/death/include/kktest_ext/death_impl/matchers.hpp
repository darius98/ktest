#pragma once

#include <kktest_ext/matchers/comparison.hpp>
#include <kktest_ext/matchers/composite.hpp>
#include <kktest_ext/matchers/matcher.hpp>
#include <kktest_ext/death_impl/check_death.hpp>
#include <kktest_ext/death_impl/death_status.hpp>

namespace kktest::death {

namespace detail {
class ExitsMatcher;
class HasExitedMatcher;
template<class T> class HasExitedWithCodeMatcher;
template<class T> class HasExitedWithOutputMatcher;
}

template<class T> auto hasExitedWithCode(const T& exitCode) {
    if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
        return detail::HasExitedWithCodeMatcher<T>(exitCode);
    } else {
        return detail::HasExitedWithCodeMatcher(matchers::isEqualTo(exitCode));
    }
}

template<class T> auto hasExitedWithOutput(const T& output) {
    if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
        return detail::HasExitedWithOutputMatcher<T>(output);
    } else {
        return detail::HasExitedWithOutputMatcher(matchers::isEqualTo(output));
    }
}

namespace detail {

inline void describeStatus(matchers::Description* description,
                           const DeathStatus& status) {
    if (status.killedBySignal()) {
        (*description) << "the program's termination by signal "
                       << status.getSignal();
    } else if (status.exited()) {
        (*description) << "the program's end with code "
                       << status.getExitCode();
    } else {
        (*description) << "not a program exit";
    }
}

template<class CM, class OM>
struct ExitsWithCodeAndOutputState {
    bool codeMatcherMatches = false;
    typename CM::State codeMatcherState;

    bool outputMatcherMatches = false;
    typename OM::State outputMatcherState;

    DeathStatus status;
};

template<class CM, class OM>
class ExitsWithCodeAndOutputMatcher: public matchers::StatefulMatcher<
            ExitsWithCodeAndOutputState<CM, OM>> {
    static_assert(std::is_base_of<matchers::Matcher, CM>::value,
                  "ExitsWithCodeAndOutputMatcher only supports matchers as "
                  "template arguments.");
    static_assert(std::is_base_of<matchers::Matcher, OM>::value,
                  "ExitsWithCodeAndOutputMatcher only supports matchers as "
                  "template arguments.");
 public:
    constexpr ExitsWithCodeAndOutputMatcher(const CM& codeMatcher,
                                            const OM& outputMatcher):
            codeMatcher(codeMatcher), outputMatcher(outputMatcher) {}

    bool matches(const Executable& func,
                 ExitsWithCodeAndOutputState<CM, OM>* state) const {
        state->status = checkDeath(func);
        if constexpr (CM::HasState) {
            state->codeMatcherMatches = codeMatcher.matches(
                    state->status.getExitCode(), &state->codeMatcherState);
        } else {
            state->codeMatcherMatches= codeMatcher.matches(
                    state->status.getExitCode());
        }
        if constexpr (OM::HasState) {
            state->outputMatcherMatches = outputMatcher.matches(
                    state->status.getOutput(), &state->outputMatcherState);
        } else {
            state->outputMatcherMatches = outputMatcher.matches(
                    state->status.getOutput());
        }
        return state->codeMatcherMatches && state->outputMatcherMatches;
    }

    void describe(matchers::Description* description) const override {
        (*description) << "the program's end with code that is ";
        codeMatcher.describe(description);
        (*description) << " and where the final output is ";
        outputMatcher.describe(description);
    }

    void describeFailure(
            matchers::Description* description,
            ExitsWithCodeAndOutputState<CM, OM>* state) const override {
        if (!state->codeMatcherMatches) {
            describeStatus(description, state->status);
        } else {
            (*description) << "the program's end with valid return code,"
                              " but output is ";
            if constexpr (OM::HasState) {
                outputMatcher.describeFailure(
                        description, &state->outputMatcherState);
            } else {
                outputMatcher.describeFailure(description);
            }
        }
    }

 private:
    CM codeMatcher;
    OM outputMatcher;
};

template<class M>
struct ExitsWithCodeState {
    DeathStatus status;
    typename M::State codeMatcherState;
};

template<class M>
class ExitsWithCodeMatcher:
        public matchers::StatefulMatcher<ExitsWithCodeState<M>> {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "ExitsWithCodeMatcher only supports matchers as template "
                  "arguments.");
 public:
    explicit constexpr ExitsWithCodeMatcher(const M& codeMatcher):
            codeMatcher(codeMatcher) {}

    bool matches(const Executable& func, ExitsWithCodeState<M>* state) const {
        state->status = checkDeath(func);
        if constexpr (M::HasState) {
            return codeMatcher.matches(
                    state->status.getExitCode(), &state->codeMatcherState);
        } else {
            return codeMatcher.matches(state->status.getExitCode());
        }
    }

    void describe(matchers::Description* description) const override {
        (*description) << "the program's end with code that is ";
        codeMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description,
                         ExitsWithCodeState<M>* state) const override {
        describeStatus(description, state->status);
    }

    template<class T>
    auto withOutput(const T& output) const {
        if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
            return ExitsWithCodeAndOutputMatcher(codeMatcher, output);
        } else {
            return ExitsWithCodeAndOutputMatcher(
                    codeMatcher, matchers::isEqualTo(output));
        }
    }

 private:
    M codeMatcher;
};

template<class M>
struct ExitsWithOutputState {
    typename M::State outputMatcherState;
    DeathStatus status;
};

template<class M>
class ExitsWithOutputMatcher:
        public matchers::StatefulMatcher<ExitsWithOutputState<M>> {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "ExitsWithCodeMatcher only supports matchers as template "
                  "arguments.");
 public:
    explicit constexpr ExitsWithOutputMatcher(const M& outputMatcher):
            outputMatcher(outputMatcher) {}

    bool matches(const Executable& func, ExitsWithOutputState<M>* state) const {
        state->status = checkDeath(func);
        if constexpr (M::HasState) {
            return outputMatcher.matches(
                    state->status.getOutput(), &state->outputMatcherState);
        } else {
            return outputMatcher.matches(state->status.getOutput());
        }
    }

    void describe(matchers::Description* description) const override {
        (*description) << "the program's end, where the final output is ";
        outputMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description,
                         ExitsWithOutputState<M>* state) const override {
        if constexpr (M::HasState) {
            outputMatcher.describeFailure(
                    description, &state->outputMatcherState);
        } else {
            outputMatcher.describeFailure(description);
        }
    }

    template<class T>
    auto withCode(const T& code) const {
        if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
            return ExitsWithCodeAndOutputMatcher(code, outputMatcher);
        } else {
            return ExitsWithCodeAndOutputMatcher(
                    matchers::isEqualTo(code), outputMatcher);
        }
    }

 private:
    M outputMatcher;
};

class ExitsMatcher: public matchers::StatefulMatcher<DeathStatus> {
 public:
    constexpr ExitsMatcher():
            zero(matchers::isZero), nonZero(matchers::isNotZero) {}

    bool matches(const Executable& func, DeathStatus* state) const {
        *state = checkDeath(func);
        return state->exited();
    }

    void describe(matchers::Description* description) const override {
        (*description) << "the program's end";
    }

    void describeFailure(matchers::Description* description,
                         DeathStatus* state) const override {
        describeStatus(description, *state);
    }

    template<class T>
    auto withCode(const T& code) const {
        if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
            return ExitsWithCodeMatcher<T>(code);
        } else {
            return ExitsWithCodeMatcher(matchers::isEqualTo(code));
        }
    }

    template<class T>
    auto withOutput(const T& output) const {
        if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
            return ExitsWithOutputMatcher<T>(output);
        } else {
            return ExitsWithOutputMatcher(matchers::isEqualTo(output));
        }
    }

    ExitsWithCodeMatcher<matchers::detail::IsZeroMatcher> zero;
    ExitsWithCodeMatcher<matchers::detail::IsNotZeroMatcher> nonZero;
};

class HasExitedMatcher: public matchers::StatefulMatcher<const DeathStatus*> {
 public:
    bool matches(const DeathStatus& status, const DeathStatus** state) const {
        *state = &status;
        return status.exited();
    }

    void describe(matchers::Description* description) const override {
        (*description) << "the program's end";
    }

    void describeFailure(matchers::Description* description,
                         const DeathStatus** state) const override {
        describeStatus(description, **state);
    }
};

template<class M>
class HasExitedWithCodeMatcher:
        public matchers::StatefulMatcher<const DeathStatus*> {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "HasExitedWithCodeMatcher only supports matchers as template "
                  "arguments.");
 public:
    explicit constexpr HasExitedWithCodeMatcher(const M& exitCodeMatcher):
            exitCodeMatcher(exitCodeMatcher) {}

    bool matches(const DeathStatus& status, const DeathStatus** state) const {
        *state = &status;
        if constexpr (M::HasState) {
            typename M::State codeMatcherState;
            return status.exited() && exitCodeMatcher.matches(
                    status.getExitCode(), &codeMatcherState);
        } else {
            return status.exited() && exitCodeMatcher.matches(
                    status.getExitCode());
        }
    }

    void describe(matchers::Description* description) const override {
        (*description) << "the program's end with code that is ";
        exitCodeMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description,
                          const DeathStatus** status) const override {
        describeStatus(description, **status);
    }

 private:
    M exitCodeMatcher;
};

template<class M>
class HasExitedWithOutputMatcher:
        public matchers::StatefulMatcher<typename M::State> {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "HasExitedWithOutputMatcher only supports matchers as template "
                  "arguments.");

 public:
    explicit HasExitedWithOutputMatcher(M outputMatcher):
            outputMatcher(std::move(outputMatcher)) {}

    bool matches(const DeathStatus& status, typename M::State* state) const {
        if constexpr (M::HasState) {
            return outputMatcher.matches(status.getOutput(), state);
        } else {
            return outputMatcher.matches(status.getOutput());
        }
    }

    void describe(matchers::Description* description) const override {
        (*description) << "the program's end, where the final output is ";
        outputMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description,
                         typename M::State* state) const override {
        if constexpr (M::HasState) {
            outputMatcher.describeFailure(description, state);
        } else {
            outputMatcher.describeFailure(description);
        }
    }

 private:
    M outputMatcher;
};

}

constexpr detail::ExitsMatcher exits;

constexpr detail::HasExitedMatcher hasExited;

constexpr detail::HasExitedWithCodeMatcher<matchers::detail::IsZeroMatcher>
        hasExitedWithCodeZero(matchers::isZero);

constexpr detail::HasExitedWithCodeMatcher<matchers::detail::IsNotZeroMatcher>
        hasExitedWithNonZeroCode(matchers::isNotZero);

}
