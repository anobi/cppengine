#ifndef VECTOR_H
#define VECTOR_H

class vector3{
public:
    float x, y, z;
    vector3();
    explicit vector3(const float x, const float y, const float z);
    void Set(const float x, const float y, const float z);
    void Zero();

    float   operator [](int i) const;
    float & operator [](int i);
    vector3 operator + (const vector3 &v) const;
    vector3 operator - (const vector3 &v) const;
    vector3 operator * (const vector3 &v) const;
    vector3 operator * (const float a) const;
    vector3 operator / (const float av) const;
};

inline vector3::vector3(){}
inline vector3::vector3(const float x, const float y, const float z){
    this->x = x;
    this->y = y;
    this->z = z;
}
inline void vector3::Set(const float x, const float y, const float z){
    this->x = x;
    this->y = y;
    this->z = z;
}
inline void vector3::Zero(){
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

#endif
