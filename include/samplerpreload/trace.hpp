#pragma once

#include <span>
#include <vector>

#include <signalsafe/time.hpp>

#include <signalsampler/backtrace.hpp>

namespace samplerpreload {
    class Trace {
    public:
        virtual ~Trace() = default;

        static Trace from(std::span<const unsigned char>);

        struct Sample final {
            signalsafe::time::TimeSpecification timestamp;
            std::vector<signalsampler::instruction_pointer_t> backtrace;
        };

        std::vector<Sample> get_samples() const;

    private:
        std::vector<Sample> m_samples;
    };
}
