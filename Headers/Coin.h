#pragma once

#include "Vector3d.h"
#include "CollisionBox.h"

class Coin {
public:
	Coin(Vector3d position, GLuint frontTexture, GLuint coreTexture, GLuint backTexture);
	Coin(Vector3d position, GLuint frontTexture, GLuint coreTexture, GLuint backTexture, float radius, float width, int precision);
	~Coin();
	void updateRotation();
	CollisionBox getCollisionBox();

private:
	Vector3d position;
	GLuint frontTexture;
	GLuint coreTexture;
	GLuint backTexture;
	float radius;
	float width;
	int precision;
	float angle;
	CollisionBox collisionBox;
};

