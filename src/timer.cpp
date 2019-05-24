#include "timer.h"
#include <GLFW/glfw3.h>

Timer::Timer(double interval) {
    prev = 0;
    this->interval = interval;
}

bool Timer::processTick() {
    cur = glfwGetTime();
    if (cur - prev >= interval) {
        prev = cur;
        return true;
    } else
        return false;
}

void Timer::reset(int interval) {
    prev = cur = glfwGetTime();
    this->interval = interval;
}
