#ifndef VECTOR_H
#define VECTOR_H

template <class T>
class vector3 {
public:

    T x, y, z;
    vector3();

    inline bool operator == (const vector3 &v) const {
        return (x == v.x && y == v.y && z == v.z);
    };
    inline vector3 operator + (const vector3 &v) const{
        return vector3(x + v.x, y + v.y, z + v.z);
    };
    inline vector3 operator - (const vector3 &v) const{
        return vector3(x - v.x, y - v.y, z - v.z);
    };
    inline vector3 operator * (const vector3 &v) const{
        return vector3(x * v.x, y * v.y, z * v.z);
    };
    inline vector3 operator / (const vector3 &v) const{
        return vector3(x / v.x, y / v.y, z / v.z);
    };
    inline vector3 operator + (const T a) const{
        return vector3(x + a, y + a, z + a);
    };
    inline vector3 operator - (const T a) const{
        return vector3(x - a, y - a, z - a);
    };
    inline vector3 operator * (const T a) const{
        return vector3(x * a, y * a, z * a);
    };
    inline vector3 operator / (const T a) const{
        return vector3(x / a, y / a, z / a);
    };

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
    inline T Length(){
        return T(sqrt(x*x + y*y + z*z));
    };
};

#endif
