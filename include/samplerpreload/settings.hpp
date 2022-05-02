#pragma once

namespace samplerpreload {
    class Settings {
    public:
        double get_samples_per_second() const;
        void set_samples_per_second(double);

        static Settings read_from_env();
        void write_to_env();

    private:
        double m_samplesPerSecond;
    };
}
