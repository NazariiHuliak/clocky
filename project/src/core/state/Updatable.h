#ifndef UPDATABLE_H
#define UPDATABLE_H

class Updatable {
public:
    virtual void update(unsigned long updateTime) = 0;
    virtual ~Updatable() {}
};

#endif
