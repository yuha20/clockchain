//
// reference from https://github.com/99x/timercpp
//One issue is the very fact that it uses threads. Indeed, the JavaScript equivalent uses an event loop, and does not create a new thread for each invocation of setTimeout or setInterval.

//Also, the clear flag is read and written from several threads, and  correct me if I’m wrong  there is nothing to protect it from a race condition.
//

#ifndef PROJECT3_TIMER_H
#define PROJECT3_TIMER_H
#include <iostream>
#include <thread>
#include <chrono>

class Timer {
    bool clear = false;

public:
    template<class A>
    void setTimeout(A function, int delay);
    template<class A>
    void setInterval(A function, int interval);
    void stopTimer();

};
template<class A>
void Timer::setTimeout(A function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if(this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(this->clear) return;
        function();
    });
    t.detach();
}
template<class A>
void Timer::setInterval(A function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(this->clear) return;
            function();
        }
    });
    t.detach();
}

void Timer::stopTimer() {
    this->clear = true;
}
#endif //PROJECT3_TIMER_H
