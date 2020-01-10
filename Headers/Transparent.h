#pragma once

#include "Vector3d.h"
#include "CollisionBox.h"

class Transparent{
public:
	Transparent();
	Transparent(Vector3d position, Vector3d triangle1, Vector3d triangle2, Vector3d triangle3, Vector3d color, double alpha, float textureFactor, GLuint texture);
	~Transparent();
	void drawTransparent();
	void setCollision();
	bool isHit() const;
	bool isDestructible() const;
	bool isHarmful() const;
	CollisionBox getCollisionBox() const;
	void setPosition(Vector3d newPosition);
	Vector3d getPosition() const;
	void calcCentroid();

private:
	Vector3d position;
	Vector3d triangle1;
	Vector3d triangle2;
	Vector3d triangle3;
	Vector3d normal;
	Vector3d color;
	Vector3d center;
	double alpha;

	GLuint texture;
	float textureFactor;
	bool hit;
	bool destructible;
	bool harmful;
	CollisionBox collisionBox;
};