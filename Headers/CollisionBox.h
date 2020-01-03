#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "Vector3d.h"

class CollisionBox {
public:
	CollisionBox();
	CollisionBox(Vector3d position, double halfSideLength); // Cube
	CollisionBox(Vector3d position, Vector3d halfLengths); // Rectangular form
	~CollisionBox();

	Vector3d getPosition();
	double getX();
	double getY();
	double getZ();

	Vector3d getSize();

	static GLboolean CheckCollision(CollisionBox one, CollisionBox two); // AABB - AABB collision

	void debugDrawBox();

private:
	Vector3d position;
	Vector3d size;
};

#endif
