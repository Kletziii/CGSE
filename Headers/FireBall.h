#pragma once

#include "Vector3d.h"
#include "CollisionBox.h"

class FireBall {
public:
	FireBall(Vector3d position, Vector3d lookAt, float speed, GLuint texture);
	~FireBall();
	void updatePosition();
	float getDistanceTraveled();
	void setCollision();
	bool isHit();
	CollisionBox getCollisionBox();

private:
	Vector3d position;
	Vector3d direction; // normalized
	float distanceTraveled;
	float speed;
	GLuint texture;
	bool hit;
	CollisionBox collisionBox;
};