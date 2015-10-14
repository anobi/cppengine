#ifndef TYPES_H
#define TYPES_H

class vector3{
public:
    float x, y, z;
    vector3();
    vector3(const float x, const float y, const float z);
    void Set(const float x, const float y, const float z);
};

#endif
