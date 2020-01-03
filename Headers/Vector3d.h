// Credits: https://gist.github.com/zooty/f2222e0bc33e6845f068

#pragma once

class Vector3d {
private:
	double x;
	double y;
	double z;

public:
	Vector3d();
	Vector3d(double, double, double);

	double getX();
	double getY();
	double getZ();

	double length();
	Vector3d normalized();
	Vector3d neg();
	double dot(Vector3d other);
	Vector3d cross(Vector3d other);

	Vector3d operator + (const Vector3d &other);
	Vector3d operator - (const Vector3d &other);
	Vector3d operator * (const Vector3d &other);
	Vector3d operator / (const Vector3d &other);

	// added overload
	Vector3d operator * (float f);
};