#include "samplerpreload-utils-test.hpp"

#include "samplerpreload/settings.hpp"

using samplerpreload::Settings;

SCENARIO("samplerpreload::Settings", "[settings]") {
    GIVEN("settings") {
        Settings settings;

        WHEN("samples per second is set to 42") {
            settings.set_samples_per_second(42);

            THEN("its getter returns 42") {
                REQUIRE(settings.get_samples_per_second());
            }

            AND_WHEN("it is written to the environment") {
                settings.write_to_env();

                AND_WHEN("settings are read back from the environment") {
                    const auto readSettings = Settings::read_from_env();

                    THEN("they too have 42 set as samples per second") {
                        REQUIRE(readSettings.get_samples_per_second() == 42);
                    }
                }
            }
        }
    }
}
