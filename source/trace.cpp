#include <samplerpreload/trace.hpp>

#include <cassert>
#include <fstream>

using samplerpreload::Trace;

using signalsampler::instruction_pointer_t;

namespace {
    template <typename T>
    void read_and_advance(T& sink, std::span<const unsigned char>& data) {
        assert(data.size() >= sizeof(T));
        sink = *reinterpret_cast<const T*>(data.data());
        data = data.subspan(sizeof(T));
    }
}

Trace Trace::from(std::span<const unsigned char> data) {
    Trace trace;

    while(! data.empty()) {
        Sample sample;
        read_and_advance(sample.timestamp.seconds, data);
        read_and_advance(sample.timestamp.nanoseconds, data);

        instruction_pointer_t ip{0};

        while(true) {
            read_and_advance(ip, data);
            if (ip == 0) {
                break;
            }

            sample.backtrace.push_back(ip);
        }

        trace.m_samples.push_back(sample);
    }

    return trace;
}

std::vector<Trace::Sample> Trace::get_samples() const {
    return m_samples;
}

extern "C" {
    void* samplerpreload_trace_from(const unsigned char* const data, const size_t dataLength) {
        return new Trace(Trace::from({data, dataLength}));
    }

    void samplerpreload_trace_dtor(void* const trace) {
        reinterpret_cast<Trace*>(trace)->~Trace();
    }

    size_t samplerpreload_trace_get_sample_count(const void* const trace) {
        return reinterpret_cast<const Trace*>(trace)->m_samples.size();
    }

    const void* samplerpreload_trace_get_sample_reference(const void* const trace, const size_t n) {
        return &(reinterpret_cast<const Trace*>(trace)->m_samples[n]);
    }

    int64_t samplerpreload_sample_get_timestamp_seconds(const void* const sample) {
        return reinterpret_cast<const Trace::Sample*>(sample)->timestamp.seconds;
    }

    int64_t samplerpreload_sample_get_timestamp_nanoseconds(const void* const sample) {
        return reinterpret_cast<const Trace::Sample*>(sample)->timestamp.nanoseconds;
    }

    size_t samplerpreload_sample_get_backtrace_size(const void* const sample) {
        return reinterpret_cast<const Trace::Sample*>(sample)->backtrace.size();
    }

    uint64_t samplerpreload_sample_get_backtrace_entry(const void* const sample, const size_t n) {
        return reinterpret_cast<const Trace::Sample*>(sample)->backtrace[n];
    }
}
