#ifndef TRANSITIONABLEWITHDIRECTION_H
#define TRANSITIONABLEWITHDIRECTION_H

#include "BaseTransitionable.h"

class MultiInitiationTransitionable : public BaseTransitionable {
public:
    virtual void initiateTransition(bool direction) = 0;
    virtual void initiateTransition(uint8_t nextValue) = 0;
protected:
    bool transitionDirection = false;
};

#endif