#pragma once

#include <SDL3/SDL_stdinc.h>

namespace engine::core {

class Time final {
private:
    uint64_t _last_time = 0;
    uint64_t _frame_start_time = 0;
    double _delta_time = 0.0;
    double _time_scale = 1.0;

    int _target_fps = 0;
    double _target_frame_time = 0.0f;

public:
    Time();
    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;
    Time(Time&&) = delete;
    Time& operator=(Time&&) = delete;

    /**
     * @brief 每帧开始调用，并且更新_delta_time的值
     */
    void update();

    /**
     * @brief  获取当前帧的delta_time
     * @return  缩放后的delta_time
     */
    double getDeltaTime() const;

    /**
     * @brief  获取当前帧的delta_time
     * @return  未缩放的delta_time
     */

    double getUnscaledDeltaTime() const;
    /**
     * @brief 设置时间缩放
     * @param time_scale
     */
    void setTimeScale(double time_scale);

    /**
     * @brief  获取时间缩放
     * @return  时间缩放
     */
    double getTimeScale() const;

    /**
     * @brief 设置目标帧率
     * @param target_fps
     */
    void setTargetFPS(int target_fps);

    /**
     * @brief  获取目标帧率
     * @return  目标帧率
     *
     */
    int getTargetFPS() const;

private:
    void limitFrameRate(float current_frame_time);
};
};