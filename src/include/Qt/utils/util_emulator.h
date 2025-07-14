#include <chrono>

// TODO: Program is hard-coded to work only for NTSC, make a way to use both
constexpr const float ntsc_frame_rate = 60.0f;
constexpr const float pal_frame_rate = 50.0f;

constexpr const float frame_interval_ns = 16666666.67f;

inline std::chrono::nanoseconds framerate_to_ns(const float frame_rate)
{
    const auto ms = std::chrono::duration<double, std::milli>(1.0 / static_cast<double>(frame_rate) * 1000.0);
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(ms);

    // qDebug() << "frame_rate:" << frame_rate << "ms:" << ms.count() << "ns:" << ns.count();
    return ns;
}

inline int speed_percent(const int frame_count, const float frame_rate)
{
    return static_cast<int>(
        static_cast<float>(frame_count) / frame_rate * 100.f);
}