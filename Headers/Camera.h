#pragma once

#include "Vector3d.h"
#include "CollisionBox.h"

class Camera {
public:
	Camera(float movementSpeed);
	~Camera();
	Vector3d getPosition();
	Vector3d getLookAt();
	bool isRunning();
	void setRunning(bool running);
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void moveVertical(float distance);
	void changeLook(float mouseX, float mouseY);
	void printDebug();
	CollisionBox getCollisionBox();
	void teleportTo(Vector3d newPosition, bool resetDirection);

private:
	void updateCollisionBox();
	Vector3d position;
	Vector3d direction;
	float movementSpeed;
	bool running;
	GLfloat angleY;  /* angle of spin around y axis of scene (look left/right), in degrees */
	GLfloat angleX;  /* angle of spin around x axis of scene (look up/down), in degrees */
	CollisionBox collisionBox;
};