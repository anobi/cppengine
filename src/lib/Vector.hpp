#ifndef VECTOR_H
#define VECTOR_H

template <class T>
class vector3 {
public:

    T x, y, z;
    vector3();

    int  operator [](int i) const;
    int& operator [](int i);
    bool operator == (const vector3 &v) const;
    vector3 operator + (const vector3 &v) const;
    vector3 operator - (const vector3 &v) const;
    vector3 operator * (const vector3 &v) const;
    vector3 operator / (const vector3 &v) const;
    vector3 operator + (const T a) const;
    vector3 operator - (const T a) const;
    vector3 operator * (const T a) const;
    vector3 operator / (const T a) const;

    inline explicit vector3(const T x, const T y, const T z){
        this->x = x;
        this->y = y;
        this->z = z;
    };

    inline void Set(const T x, const T y, const T z){
        this->x = x;
        this->y = y;
        this->z = z;
    };

    inline void Zero(){
        this->x = T();
        this->y = T();
        this->z = T();
    };

    inline void Normalize(){
        
    };

    inline T Dot(){};
    inline T Length(){};
};

#endif
