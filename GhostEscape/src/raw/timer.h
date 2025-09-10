#ifndef TIMER_H
#define TIMER_H

#include "../core/object.h"

class Timer : public Object {
protected:
    float _timer = 0.0f;
    float _interval = 3.0f;
    bool _time_out = false;

public:
    static Timer* create(Object* parent, float interval = 3.0f);

    virtual void init() override;
    virtual void update(float dt) override;
    inline void setInterval(float interval) { _interval = interval; }
    inline void setTimer(float timer) { _timer = timer; }
    inline float getTimer() { return _timer; }
    inline float getInterval() { return _interval; }
    bool timeOut();
    void start() { is_active = true; }
    void stop() { is_active = false; }

    float getProcess();
};

#endif