#pragma once

#include "Vector3d.h"
#include "CollisionBox.h"

class Obstacle {
public:
	Obstacle();
	Obstacle(Vector3d position, Vector3d halfLengths, bool destructible, bool harmful, float textureFactor, GLuint texture);
	~Obstacle();
	void drawObstacle();
	void drawObstacle2();
	void setCollision();
	bool isHit() const;
	bool isDestructible() const;
	void setDestructible();
	void setLength();
	bool isHarmful() const;
	CollisionBox getCollisionBox() const;
	void setPosition(Vector3d newPosition);
	Vector3d getPosition() const;

private:
	Vector3d position;
	Vector3d halfLengths;
	GLuint texture;
	float textureFactor;
	bool hit;
	bool destructible;
	bool harmful;
	CollisionBox collisionBox;
};