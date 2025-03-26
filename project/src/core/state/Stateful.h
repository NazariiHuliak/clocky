#ifndef STATEFUL_H
#define STATEFUL_H

#include "IStateful.h"

template <typename DATA_TYPE>
struct Stateful : public IStateful {
    DATA_TYPE data;
};

#endif
