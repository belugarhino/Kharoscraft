#pragma once

#include <chrono>

namespace khc::core {

class Clock {
public:
    using clock = std::chrono::steady_clock;

    void reset() { last_ = clock::now(); }

    [[nodiscard]] float tick() {
        auto now = clock::now();
        std::chrono::duration<float> delta = now - last_;
        last_ = now;
        return delta.count();
    }

private:
    clock::time_point last_{clock::now()};
};

} // namespace khc::core
