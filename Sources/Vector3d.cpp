// Credits: https://gist.github.com/zooty/f2222e0bc33e6845f068

#include <cmath>
#include "../Headers/Vector3d.h"

Vector3d::Vector3d() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3d::Vector3d(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3d::getX() { return x; }
double Vector3d::getY() { return y; }
double Vector3d::getZ() { return z; }

double Vector3d::length() {
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3d Vector3d::normalized() {
	double mag = length();

	return Vector3d(x / mag, y / mag, z / mag);
}

Vector3d Vector3d::neg() {
	return Vector3d(-x, -y, -z);
}

double Vector3d::dot(Vector3d other) {
	return x * other.getX() + y * other.getY() + z * other.getZ();
}

Vector3d Vector3d::cross(Vector3d other) {
	double x_ = y * other.getZ() - z * other.getY();
	double y_ = z * other.getX() - x * other.getZ();
	double z_ = x * other.getY() - y * other.getX();

	return Vector3d(x_, y_, z_);
}

Vector3d Vector3d::operator + (const Vector3d &other) {
	return Vector3d(x + other.x, y + other.y, z + other.z);
}

Vector3d Vector3d::operator - (const Vector3d &other) {
	return Vector3d(x - other.x, y - other.y, z - other.z);
}

Vector3d Vector3d::operator * (const Vector3d &other) {
	return Vector3d(x * other.x, y * other.y, z * other.z);
}

Vector3d Vector3d::operator / (const Vector3d &other) {
	return Vector3d(x / other.x, y / other.y, z / other.z);
}

// added overload
Vector3d Vector3d::operator * (float f) {
	return Vector3d(x * f, y * f, z * f);
}