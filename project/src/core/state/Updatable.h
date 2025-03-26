#ifndef UPDATABLE_H
#define UPDATABLE_H

class Updatable {
public:
    virtual ~Updatable() {}
    virtual void update(unsigned long updateTime) = 0;
    virtual bool isUpdating() = 0;
    virtual bool canBeUpdateAt(unsigned long updateTime) = 0;
};

#endif
