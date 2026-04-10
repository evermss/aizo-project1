#ifndef LINEARSTRUCTURE_H
#define LINEARSTRUCTURE_H

class LinearStructure {
public:
    virtual ~LinearStructure() = default;

    virtual int getSize() const = 0;
    virtual void clear() = 0;
    virtual void pushBack(int value) = 0;
    virtual int get(int index) const = 0;
    virtual void set(int index, int value) = 0;
};

#endif