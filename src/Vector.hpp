#ifndef VECTOR_H
#define VECTOR_H

class vector3 {
public:

    float x, y, z;

	vector3();

    explicit vector3(const float x, const float y, const float z);

    bool operator == (const vector3 &v);
    vector3 operator + (const vector3 &v);
    vector3 operator - (const vector3 &v);
    vector3 operator * (const vector3 &v);
    vector3 operator / (const vector3 &v);
    vector3 operator + (const float a);
    vector3 operator - (const float a);
    vector3 operator * (const float a);
    vector3 operator / (const float a);

    void Set(const float x, const float y, const float z);
    void Zero();
    void Normalize();
    float Dot();
    float Length();
};

#endif
