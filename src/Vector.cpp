#include <math.h>
#include "Vector.hpp"

vector3::vector3(){
	
}

vector3::vector3(const float xx, float yy, const float zz){
	x = xx;
	y = yy;
	z = zz;
}

void vector3::vector3::Set(const float ax, float ay, const float az){
	x = ax;
	this->y = ay;
	this->z = az;
}

void vector3::vector3::Zero(){
	this->x = float();
	this->y = float();
	this->z = float();
}

void vector3::vector3::Normalize(){
        
}

float vector3::vector3::Dot(){
	return 0.0f;
}

float vector3::vector3::Length(){
	return float(sqrt(x*x + y*y + z*z));
}

bool vector3::operator == (const vector3 &v) {
	return (x == v.x && y == v.y && z == v.z);
};
vector3 vector3::operator + (const vector3 &v) {
	return vector3(x + v.x, y + v.y, z + v.z);
};
vector3 vector3::operator - (const vector3 &v){
	return vector3(x - v.x, y - v.y, z - v.z);
};
vector3 vector3::operator * (const vector3 &v){
	return vector3(x * v.x, y * v.y, z * v.z);
};
vector3 vector3::operator / (const vector3 &v){
	return vector3(x / v.x, y / v.y, z / v.z);
};
vector3 vector3::operator + (const float a){
	return vector3(x + a, y + a, z + a);
};
vector3 vector3::operator - (const float a){
	return vector3(x - a, y - a, z - a);
};
vector3 vector3::operator * (const float a){
	return vector3(x * a, y * a, z * a);
};
vector3 vector3::operator / (const float a){
	return vector3(x / a, y / a, z / a);
};
