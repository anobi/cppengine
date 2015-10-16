#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

template <typename T>
class vector3 {
public:

    T x, y, z;

	vector3(){};

	explicit vector3(const T xx, T yy, const T zz){
		this->x = xx;
		this->y = yy;
		this->z = zz;
	}

	inline bool operator == (const vector3 &v) const {
		return (x == v.x && y == v.y && z == v.z);
	};
	inline vector3 operator + (const vector3 &v) const {
		return vector3(x + v.x, y + v.y, z + v.z);
	};
	inline vector3 operator - (const vector3 &v) const {
		return vector3(x - v.x, y - v.y, z - v.z);
	};
	inline vector3 operator * (const vector3 &v) const {
		return vector3(x * v.x, y * v.y, z * v.z);
	};
	inline vector3 operator / (const vector3 &v) const {
		return vector3(x / v.x, y / v.y, z / v.z);
	};
	inline vector3 operator + (const T a) const {
		return vector3(x + a, y + a, z + a);
	};
	inline vector3 operator - (const T a) const {
		return vector3(x - a, y - a, z - a);
	};
	inline vector3 operator * (const T a) const {
		return vector3(x * a, y * a, z * a);
	};
	inline vector3 operator / (const T a) const {
		return vector3(x / a, y / a, z / a);
	};

	inline void Set(const T x, T y, const T z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline void Zero(){
		this->x = T();
		this->y = T();
		this->z = T();
	}

	inline void Normalize(){
        x = x / Length();
        y = y / Length();
        z = z / Length();
	}

	inline T Dot(vector3<T> &v){
		return T(x * v.x + y * v.y + z * v.z);
	}

	inline T Length(){
		return T(sqrt(x*x + y*y + z*z));
	}
};

#endif
