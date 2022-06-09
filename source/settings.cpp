#include <samplerpreload/settings.hpp>

#include <cstdlib>
#include <string>

using samplerpreload::Settings;

namespace {
    const char* get_env_var_or_default(const char* const envVarName, const char* const defaultValue) {
        const auto envVarValue = getenv(envVarName);
        return envVarValue == nullptr ? defaultValue : envVarValue; 
    }

    constexpr char samplesPerSecondEnvVar[] = "SAMPLERPRELOAD_SAMPLES_PER_SECOND";
    constexpr char traceFilePathEnvVar[] = "SAMPLERPRELOAD_TRACE_FILE_PATH";
}

double Settings::get_samples_per_second() const {
    return m_samplesPerSecond;
}

void Settings::set_samples_per_second(const double samplesPerSecond) {
    m_samplesPerSecond = samplesPerSecond;
}

std::filesystem::path Settings::get_trace_file_path() const {
    return m_traceFilePath;
}

void Settings::set_trace_file_path(const std::filesystem::path path) {
    m_traceFilePath = path;
}

Settings Settings::read_from_env() {
    Settings settings;

    settings.set_samples_per_second(std::stod(get_env_var_or_default(samplesPerSecondEnvVar, "0")));
    settings.set_trace_file_path(get_env_var_or_default(traceFilePathEnvVar, ""));

    return settings;
}

void Settings::write_to_env() {
    setenv(samplesPerSecondEnvVar, std::to_string(get_samples_per_second()).c_str(), 1);
    setenv(traceFilePathEnvVar, get_trace_file_path().c_str(), 1);
}

// C API

extern "C" {
    void* samplerpreload_settings_ctor() {
        return new Settings;
    }

    void samplerpreload_settings_dtor(void* const instance) {
        reinterpret_cast<Settings*>(instance)->~Settings();
    }

    void samplerpreload_settings_set_samples_per_second(void* const instance, double samplesPerSecond) {
        reinterpret_cast<Settings*>(instance)->set_samples_per_second(samplesPerSecond);
    }

    void samplerpreload_settings_set_trace_file_path(void* const instance, const char* const traceFilePath) {
        reinterpret_cast<Settings*>(instance)->set_trace_file_path(traceFilePath);
    }

    void samplerpreload_settings_write_to_env(void* const instance) {
        reinterpret_cast<Settings*>(instance)->write_to_env();
    }
}
