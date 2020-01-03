#pragma once

#include "Vector3d.h"
#include "CollisionBox.h"

class Wall {
public:
	Wall(float beginX, float beginZ, float endX, float endZ, float height, bool negateNormal, float textureFactor, GLuint texture);
	~Wall();
	void draw();
	CollisionBox getCollisionBox();

private:
	void calculateNormal(bool negateNormal);
	Vector3d beginPosition;
	Vector3d endPosition;
	Vector3d normal;
	float textureFactor;
	GLuint texture;
	CollisionBox collisionBox;
};

