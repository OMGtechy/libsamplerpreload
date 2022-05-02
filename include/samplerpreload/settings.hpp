#pragma once

#include <filesystem>

namespace samplerpreload {
    class Settings {
    public:
        double get_samples_per_second() const;
        void set_samples_per_second(double);

        std::filesystem::path get_trace_file_path() const;
        void set_trace_file_path(std::filesystem::path);

        static Settings read_from_env();
        void write_to_env();

    private:
        double m_samplesPerSecond;
        std::filesystem::path m_traceFilePath;
    };
}
