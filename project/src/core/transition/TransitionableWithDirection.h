#ifndef TRANSITIONABLEWITHDIRECTION_H
#define TRANSITIONABLEWITHDIRECTION_H

#include "BaseTransitionable.h"

class TransitionableWithDirection : public BaseTransitionable {
public:
    virtual void initiateTransition(bool direction) = 0;

protected:
    bool transitionDirection = false;
};

#endif