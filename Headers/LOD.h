#pragma once

#include "Vector3d.h"
#include "CollisionBox.h"

class LOD {
public:
	LOD();
	LOD(Vector3d position, Vector3d halfLengths, bool destructible, bool harmful, float textureFactor, GLuint texture);
	~LOD();
	void drawLOD();
	void setCollision();
	bool isHit() const;
	bool isDestructible() const;
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