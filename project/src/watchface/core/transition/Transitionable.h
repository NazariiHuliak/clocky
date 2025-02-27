#ifndef TRANSITIONABLE_H
#define TRANSITIONABLE_H

#include "BaseTransitionable.h"

class Transitionable : public BaseTransitionable {
public:
    virtual void initiateTransition(uint8_t nextIndex) = 0;
};

#endif // TRANSITIONABLE_H
