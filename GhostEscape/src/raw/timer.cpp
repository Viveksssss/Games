#include "timer.h"

void Timer::update(float dt)
{
    _timer += dt;
    if (_timer > _interval) {
        _timer = 0;
        _time_out = true;
    }
}

bool Timer::timeOut()
{
    if (_time_out) {
        _time_out = false;
        return true;
    }
    return false;
}

void Timer::init()
{
    Object::init();
    is_active = false;
}

Timer* Timer::create(Object* parent, float interval)
{
    auto timer = new Timer();
    timer->init();
    timer->setInterval(interval);
    if (parent) {
        parent->addChild(timer);
    }

    return timer;
}