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

	inline void Set(const T ax, T ay, const T az){
		x = ax;
		this->y = ay;
		this->z = az;
	}

	inline void Zero(){
		this->x = T();
		this->y = T();
		this->z = T();
	}

	inline void Normalize(){

	}

	inline T Dot(){
		return 0.0f;
	}

	inline T Length(){
		return T(sqrt(x*x + y*y + z*z));
	}
};

#endif
