#include <stdlib.h> // for exit (must be above GL)
#include <cmath>
#include <iostream>
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/Camera.h"

#ifndef M_PI
#define M_PI 3.141592653
#endif

#define RAD(x) (((x)*M_PI)/180.)

Camera::Camera(float movementSpeed) {
	this->position = Vector3d(0, 1, 0);
	this->direction = Vector3d(0, 0, -1);
	this->movementSpeed = movementSpeed;
	this->running = false;
	this->angleY = 0;
	this->angleX = 0;

	updateCollisionBox();
}

Camera::~Camera() {
}

void Camera::updateCollisionBox() {
	Vector3d collisionBoxPosition = Vector3d(
		position.getX(),
		position.getY() - (1 - 1.2 / 2),
		position.getZ()
	);

	// player eyes: 1.0, player height: 1.2
	collisionBox = CollisionBox(collisionBoxPosition, Vector3d(0.25f, 0.6f, 0.25f));
}

Vector3d Camera::getPosition() {
	return position;
}

Vector3d Camera::getLookAt() {
	return position + direction;
}

void Camera::teleportTo(Vector3d newPosition, bool resetDirection) {
	position = newPosition;
	updateCollisionBox();

	if (resetDirection) {
		direction = Vector3d(0, 0, -1);
		angleX = 0;
		angleY = 0;
	}
}

void Camera::moveForward() {
	position = Vector3d(
		position.getX() + (direction.getX() * movementSpeed),
		position.getY(),
		position.getZ() + (direction.getZ() * movementSpeed)
	);

	updateCollisionBox();
}

void Camera::moveBackward() {
	position = Vector3d(
		position.getX() - (direction.getX() * movementSpeed),
		position.getY(),
		position.getZ() - (direction.getZ() * movementSpeed)
	);

	updateCollisionBox();
}

void Camera::moveLeft() {
	position = Vector3d(
		position.getX() + (direction.getZ() * movementSpeed),
		position.getY(),
		position.getZ() - (direction.getX() * movementSpeed)
	);

	updateCollisionBox();
}

void Camera::moveRight() {
	position = Vector3d(
		position.getX() - (direction.getZ() * movementSpeed),
		position.getY(),
		position.getZ() + (direction.getX() * movementSpeed)
	);

	updateCollisionBox();
}

void Camera::moveVertical(float distance) {
	position = Vector3d(
		position.getX(),
		position.getY() + distance,
		position.getZ()
	);

	updateCollisionBox();
}

bool Camera::isRunning() {
	return this->running;
}

void Camera::setRunning(bool running) {
	if (running && !this->running) {
		movementSpeed *= 2;
	}
	else if (!running && this->running) {
		movementSpeed /= 2;
	}

	this->running = running;
}

void Camera::changeLook(float mouseX, float mouseY) {
	angleX -= mouseY;
	angleY += mouseX;

	if (angleX > 90.0) angleX = 90.0;
	else if (angleX < -90.0) angleX = -90.0;

	if (angleY >= 360.0) angleY -= 360.0;
	else if (angleY < 0.0) angleY += 360.0;

	direction = Vector3d(
		sinf(RAD(angleY)),
		sinf(RAD(angleX)),
		-cosf(RAD(angleY))
	);
}

void Camera::printDebug() {
	printf("[CAMERA] posX: %.3f, posY: %.3f, posZ: %.3f, angleX: %.3f, angleY: %.3f\n", position.getX(), position.getY(), position.getZ(), angleX, angleY);
}

CollisionBox Camera::getCollisionBox() {
	return this->collisionBox;
}