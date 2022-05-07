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

    {
        uint64_t ProcMapsRangeCount = 0;
        read_and_advance(ProcMapsRangeCount, data);
        trace.m_procMaps.ranges.resize(ProcMapsRangeCount);

        for(uint64_t i = 0; i < ProcMapsRangeCount; ++i) {
            auto& range = trace.m_procMaps.ranges.at(i);
            read_and_advance(range.start, data);
            read_and_advance(range.end, data);
        }
    }

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

Trace::ProcMaps Trace::get_proc_maps() const {
    return m_procMaps;
}
