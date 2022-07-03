#pragma once

#include <span>
#include <vector>

#include <signalsafe/time.hpp>

#include <signalsampler/backtrace.hpp>

extern "C" {
    void*        samplerpreload_trace_from(const unsigned char* data, size_t dataLength);
    void         samplerpreload_trace_dtor(void* trace);
    size_t       samplerpreload_trace_get_sample_count(const void* trace);
    const void*  samplerpreload_trace_get_sample_reference(const void* trace, size_t n);

    int64_t  samplerpreload_sample_get_timestamp_seconds(const void* sample);
    int64_t  samplerpreload_sample_get_timestamp_nanoqseconds(const void* sample);
    size_t   samplerpreload_sample_get_backtrace_size(const void* sample);
    uint64_t samplerpreload_sample_get_backtrace_entry(const void* sample, size_t n);
}

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

        friend size_t      ::samplerpreload_trace_get_sample_count(const void*);
        friend const void* ::samplerpreload_trace_get_sample_reference(const void*, size_t);
    };
}
