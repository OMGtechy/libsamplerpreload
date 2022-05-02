#include "samplerpreload-test.hpp"
#include "samplerpreload/trace.hpp"

#include <array>
#include <cstddef>

using samplerpreload::Trace;

SCENARIO("samplerpreload::Trace", "[trace]") {
    GIVEN("some trace bytes") {
        constexpr std::array<unsigned char, 144> data = {
            0xdc, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xef, 0xea, 0xe6, 0x33,
            0x00, 0x00, 0x00, 0x00, 0x60, 0x56, 0x15, 0x3b, 0x68, 0x7f, 0x00, 0x00,
            0xc0, 0xa0, 0xe3, 0x3a, 0x68, 0x7f, 0x00, 0x00, 0x68, 0x9c, 0xed, 0x3a,
            0x68, 0x7f, 0x00, 0x00, 0xfb, 0xa0, 0xf5, 0xc2, 0x45, 0x56, 0x00, 0x00,
            0xb3, 0xb0, 0xe1, 0x3a, 0x68, 0x7f, 0x00, 0x00, 0x6e, 0xb0, 0xf5, 0xc2,
            0x45, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xdd, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xe0, 0xe6, 0x33,
            0x00, 0x00, 0x00, 0x00, 0x60, 0x56, 0x15, 0x3b, 0x68, 0x7f, 0x00, 0x00,
            0xc0, 0xa0, 0xe3, 0x3a, 0x68, 0x7f, 0x00, 0x00, 0x68, 0x9c, 0xed, 0x3a,
            0x68, 0x7f, 0x00, 0x00, 0xfb, 0xa0, 0xf5, 0xc2, 0x45, 0x56, 0x00, 0x00,
            0xb3, 0xb0, 0xe1, 0x3a, 0x68, 0x7f, 0x00, 0x00, 0x6e, 0xb0, 0xf5, 0xc2,
            0x45, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        WHEN("Trace::from is called") {
            const auto trace = Trace::from(data);
            const auto samples = trace.get_samples();
            
            THEN("there are 2 samples") {
                REQUIRE(samples.size() == 2);
            }

            THEN("the first sample is correct") {
                const auto s1 = samples[0];
                REQUIRE(s1.timestamp.seconds == 2524);
                REQUIRE(s1.timestamp.nanoseconds == 870771439);
                REQUIRE(s1.backtrace.size() == 5);
                REQUIRE(s1.backtrace[0] == 140085644580448);
                REQUIRE(s1.backtrace[1] == 140085641322688);
                REQUIRE(s1.backtrace[2] == 140085641976936);
                REQUIRE(s1.backtrace[3] == 94857623609595);
                REQUIRE(s1.backtrace[4] == 140085641195699);
                REQUIRE(s1.backtrace[5] == 94857623613550);
            }

            THEN("the second sample is correct") {
                const auto s2 = samples[1];
                REQUIRE(s2.timestamp.seconds == 2525);
                REQUIRE(s2.timestamp.nanoseconds == 870768701);
                REQUIRE(s2.backtrace.size() == 5);
                REQUIRE(s2.backtrace[0] == 140085644580448);
                REQUIRE(s2.backtrace[1] == 140085641322688);
                REQUIRE(s2.backtrace[2] == 140085641976936);
                REQUIRE(s2.backtrace[3] == 94857623609595);
                REQUIRE(s2.backtrace[4] == 140085641195699);
                REQUIRE(s2.backtrace[5] == 94857623613550);
            }
        }
    }

}