#ifndef SPAWNER_H
#define SPAWNER_H
#include "core/object.h"

class Player;
class Spawner : public Object {
public:
    inline void setNum(int num) { this->_num = num; }
    inline void setInterval(float interval) { this->_interval = interval; }
    inline void setTimer(float timer) { this->_timer = timer; }
    inline int getNum() const { return _num; }
    inline float getInterval() const { return _interval; }
    inline float getTimer() const { return _timer; }
    inline void setPlayer(Player* player) { this->player = player; }

    virtual void update(float dt) override;

private:
    Player* player = nullptr;

protected:
    int _num = 20;
    float _timer = 1.5f;
    float _interval = 3.0f;
};

#endif