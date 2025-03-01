#ifndef BASE_TRANSITIONABLE_H
#define BASE_TRANSITIONABLE_H

#include <Arduino.h>

class BaseTransitionable {
public:
    virtual void performTransition() = 0;
    virtual ~BaseTransitionable() = default;
    
protected:
    bool isTransitioning = false;
    int16_t transitionOffset = 0;
    uint8_t nextValue = 0;
};

#endif