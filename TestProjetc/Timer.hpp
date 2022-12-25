#pragma once
#include <chrono>

class Timer {
    typedef std::chrono::nanoseconds nanoseconds;
    typedef std::chrono::microseconds microseconds;
    typedef std::chrono::milliseconds milliseconds;
    typedef std::chrono::seconds seconds;
    typedef std::chrono::minutes minutes;
    typedef std::chrono::hours hours;
    typedef std::chrono::high_resolution_clock::time_point time_point;

public:
    void start() {
        _start_time = std::chrono::high_resolution_clock::now();
    }

    void reset() {
        start();
    }

    unsigned long nanoseconds_elapsed() {
        return std::chrono::duration_cast<nanoseconds>(_elapsed()).count();
    }

    unsigned long microseconds_elapsed() {
        return std::chrono::duration_cast<microseconds>(_elapsed()).count();
    }

    unsigned long milliseconds_elapsed() {
        return std::chrono::duration_cast<milliseconds>(_elapsed()).count();
    }

    unsigned long seconds_elapsed() {
        return std::chrono::duration_cast<seconds>(_elapsed()).count();
    }

    unsigned long minutes_elapsed() {
        return std::chrono::duration_cast<minutes>(_elapsed()).count();
    }

    unsigned long hours_elapsed() {
        return std::chrono::duration_cast<hours>(_elapsed()).count();
    }

private:
    time_point _start_time;
    nanoseconds _elapsed() {
        return std::chrono::high_resolution_clock::now() - _start_time;
    }
};