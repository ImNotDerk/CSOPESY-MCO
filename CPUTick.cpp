#include "CPUTick.h"

CPUTick* CPUTick::sharedInstance = nullptr;

CPUTick::CPUTick() {
    this->nTicks = 0;
}

CPUTick* CPUTick::getInstance() {
    if (sharedInstance == nullptr) {
        sharedInstance = new CPUTick();
    }
    return sharedInstance;
}

void CPUTick::resetTicks() {
    this->nTicks = 0;
}

void CPUTick::addTick(int ticks) {
    this->nTicks += ticks;
}

int CPUTick::getTicks() const {
    return this->nTicks;
}
