#pragma once

#include <signalsafe/file.hpp>
#include <signalsafe/time.hpp>

#include <signalsampler/backtrace.hpp>

#include <samplerpreload/trace.hpp>

namespace samplerpreload {
    class TraceFile : public signalsafe::File {
    public:
        using File::File;

        static TraceFile create_and_open(std::string_view path, Permissions permissions);

        template <std::size_t BacktraceSize>
        void add_sample(signalsafe::time::TimeSpecification&& timestamp, signalsampler::backtrace_t<BacktraceSize>&& backtrace) {
            write(timestamp.seconds);
            write(timestamp.nanoseconds);
            
            for(auto entry : backtrace) {
                if (entry == 0) {
                    break;
                }

                write(entry);
            }

            write(signalsampler::instruction_pointer_t{0});
        }
    };
}
