#include "time.h"

#include <SDL3/SDL_timer.h>
#include <spdlog/spdlog.h>

namespace engine::core {
Time::Time()
{
    _last_time = SDL_GetTicksNS();
    _frame_start_time = _last_time;
    /* 默认的fps值*/
    _target_fps = 240;
    _target_frame_time = 1.0f / (_target_fps);
}

void Time::update()
{
    _frame_start_time = SDL_GetTicksNS();
    auto current_time = static_cast<double>(_frame_start_time - _last_time) / 1000000000.0;
    if (_target_frame_time > 0.0) {
        limitFrameRate(current_time);
    } else {
        _delta_time = current_time;
    }
    _last_time = SDL_GetTicksNS();
}

double Time::getDeltaTime() const
{
    return _delta_time * _time_scale;
}

double Time::getUnscaledDeltaTime() const
{
    return _delta_time;
}

void Time::setTimeScale(double time_scale)
{
    if (time_scale < 0.0) {
        spdlog::warn("Time scale cannot be negative. Setting it to 0.0");
        _time_scale = 0.0;
    }
    _time_scale = time_scale;
}

double Time::getTimeScale() const
{
    return _time_scale;
}

void Time::setTargetFPS(int target_fps)
{
    if (target_fps < 0) {
        spdlog::warn("Can not set target FPS to negative value. Setting it to 0");
        _target_fps = 0;
        _target_frame_time = 0.0f;
    } else {
        _target_fps = target_fps;
        _target_frame_time = 1.0f / static_cast<double>(_target_fps);
    }
}

int Time::getTargetFPS() const
{
    return _target_fps;
}

void Time::limitFrameRate(float current_frame_time)
{
    if (current_frame_time < _target_frame_time) {
        double time_to_wait = _target_frame_time - current_frame_time;
        uint64_t wait_time = static_cast<uint64_t>(time_to_wait * 1.0e9);
        SDL_DelayNS(wait_time);
        _delta_time = static_cast<double>(SDL_GetTicksNS() - _last_time) / 1.0e9;
    }
}
}; // namespace engine::core
