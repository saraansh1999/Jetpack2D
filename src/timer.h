#include "main.h"

#ifndef TIMER_H
#define TIMER_H


class Timer {
public:
    Timer() {
        prev = 0;
    }
    Timer(double interval);
    double interval;
    bool processTick();
    void reset(int);
private:
    double prev, cur;
};

#endif // TIMER_H
