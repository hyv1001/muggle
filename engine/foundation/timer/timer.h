#pragma once

#include <cstdint>

namespace muggle
{
    class Timer
    {
        public:
            Timer();
            virtual ~Timer();

            double getFrequency() const;
            static std::uint64_t getTicks();
            double getSeconds() const;

            void reset();

        private:
            std::uint64_t ticksPerSecond_ { 0 };
            double secondsPerTick_ { 0.0 };
            double startSceondsNegated_ {0.0};

    };
}