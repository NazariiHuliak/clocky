#ifndef STATEFUL_H
#define STATEFUL_H

template <typename DATA_TYPE>
struct Stateful : public DATA_TYPE {
    bool isUpdating = false;
    DATA_TYPE data;
};

#endif
