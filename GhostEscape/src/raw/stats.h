#ifndef STATS_H
#define STATS_H

#include "../core/object.h"

class Stats : public Object {
protected:
    float _health;
    float _max_health;
    float _mana;
    float _max_mana;
    float _damage;

    bool is_alive;
};

#endif