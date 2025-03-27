#ifndef STATEFUL_H
#define STATEFUL_H

struct IStateful {
    bool isUpdating = false;
};

template <typename DATA_TYPE>
struct Stateful : public IStateful {
    DATA_TYPE data;
};

#endif
