#include <samplerpreload/trace-file.hpp>

using samplerpreload::Trace;
using samplerpreload::TraceFile;

TraceFile TraceFile::create_and_open(std::string_view path, Permissions permissions) {
    TraceFile traceFile;
    traceFile.create_and_open_internal(
        path,
        permissions
    );

    return traceFile;
}
