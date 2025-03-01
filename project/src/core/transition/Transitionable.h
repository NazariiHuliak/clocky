#ifndef TRANSITIONABLE_H
#define TRANSITIONABLE_H

#include "BaseTransitionable.h"

class Transitionable : public BaseTransitionable {
public:
    virtual void initiateTransition(uint8_t nextValue_) = 0;
};

#endif