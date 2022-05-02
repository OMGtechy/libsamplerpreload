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
}

double Settings::get_samples_per_second() const {
    return m_samplesPerSecond;
}

void Settings::set_samples_per_second(const double samplesPerSecond) {
    m_samplesPerSecond = samplesPerSecond;
}

Settings Settings::read_from_env() {
    Settings settings;

    settings.set_samples_per_second(std::stod(get_env_var_or_default(samplesPerSecondEnvVar, "0")));

    return settings;
}

void Settings::write_to_env() {
    setenv(samplesPerSecondEnvVar, std::to_string(get_samples_per_second()).c_str(), 1);
}
