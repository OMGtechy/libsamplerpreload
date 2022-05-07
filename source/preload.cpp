#include <atomic>
#include <cassert>
#include <cmath>
#include <fstream>

#include <signal.h>
#include <ucontext.h>

#include <signalsafe/time.hpp>

#include <signalsampler/sampler.hpp>

#include <samplerpreload/settings.hpp>
#include <samplerpreload/trace-file.hpp>

using signalsafe::time::now;

using signalsampler::get_backtrace;

using samplerpreload::Settings;
using samplerpreload::Trace;
using samplerpreload::TraceFile;

namespace {
    std::atomic_flag signalHandlerRunningFlag = ATOMIC_FLAG_INIT;
    TraceFile traceFile;
    timer_t timer;

    constexpr auto clockToUse = CLOCK_MONOTONIC;

    void signal_handler(const int, siginfo_t*, void* context) {
        if (signalHandlerRunningFlag.test_and_set()) {
            // drop samples that occur when a sample is already being taken.
            return;
        }

        traceFile.add_sample(
            now(clockToUse),
            get_backtrace<64>(
                static_cast<ucontext_t*>(context)
            )
        );

        signalHandlerRunningFlag.clear();
    }

    void setup_signal_handler(const int signal) {
        struct sigaction action;
        action.sa_sigaction = signal_handler;
        action.sa_flags = SA_SIGINFO | SA_RESTART;
        sigemptyset(&action.sa_mask);

        const auto success = sigaction(signal, &action, NULL) == 0;
        assert(success);
    }

    timer_t setup_signal_timer(const int signal) {
        sigevent signalEvent;
        signalEvent.sigev_notify = SIGEV_SIGNAL;
        signalEvent.sigev_signo = signal;

        timer_t timer;

        const auto success = timer_create(clockToUse, &signalEvent, &timer) == 0;
        assert(success);
        
        return timer;
    }

    void start_timer(timer_t timer, const double samplesPerSecond) {
        struct itimerspec timerSpec;

        const double rate = 1.0 / samplesPerSecond;
        double wholeComponent = 0.0;
        const double fractionalComponent = std::modf(rate, &wholeComponent);

        timerSpec.it_interval.tv_sec = std::llrint(wholeComponent);
        timerSpec.it_interval.tv_nsec = std::llrint(fractionalComponent * 1'000'000'000.0);
        timerSpec.it_value = timerSpec.it_interval;

        const auto success = timer_settime(timer, 0, &timerSpec, NULL) == 0;
        assert(success);
    }

    void stop_timer(timer_t timer) {
        struct itimerspec timerSpec;
        timerSpec.it_interval.tv_sec = 0;
        timerSpec.it_interval.tv_nsec = 0;
        timerSpec.it_value = timerSpec.it_interval;

        // don't assert the success of this:
        // from the user's perspective, asserting right at the end of
        // what might be a long-running program is going to be annoying at best
        timer_settime(timer, 0, &timerSpec, NULL);
    }

    Trace::ProcMaps get_proc_maps() {
        Trace::ProcMaps procMaps;

        std::ifstream procMapsFile("/proc/self/maps");

        while (true) {
            std::string rangeString;
            std::string dummy;

            procMapsFile >> rangeString;

            if (procMapsFile.eof()) {
                break;
            }

            std::getline(procMapsFile, dummy); // ignore the rest of the line

            const auto hyphenOffset = rangeString.find('-');
            assert(hyphenOffset != std::string::npos);

            const auto rangeStartString = rangeString.substr(0, hyphenOffset);
            const auto rangeEndString = rangeString.substr(hyphenOffset + 1); 

            procMaps.ranges.push_back({
                Trace::ProcMaps::Range { std::stoull(rangeStartString, 0, 16), std::stoull(rangeEndString, 0, 16) }
            });
        }

        return procMaps;
    }

    [[gnu::constructor]]
    void ctor() {
        const auto settings = Settings::read_from_env();

        traceFile = TraceFile::create_and_open(settings.get_trace_file_path().native(), TraceFile::Permissions::WriteOnly);

        traceFile.add_proc_maps(get_proc_maps());

        {
            const auto signalToUse = SIGPROF;
            setup_signal_handler(signalToUse);
            timer = setup_signal_timer(signalToUse);
        }

        start_timer(timer, settings.get_samples_per_second());
    }

    [[gnu::destructor]]
    void dtor() {
        stop_timer(timer);

        // wait for the current sample to finish, if it's still running.
        while (signalHandlerRunningFlag.test_and_set());
    }
}
