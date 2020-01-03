#include <stdlib.h> // for exit (must be above GL)
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/CollisionBox.h"
#include "../Headers/TextureLoader.h"

CollisionBox::CollisionBox() {
	// needed to avoid manual memory allocation
}

CollisionBox::CollisionBox(Vector3d position, double halfSideLength) {
	this->position = position;
	this->size = Vector3d(halfSideLength, halfSideLength, halfSideLength);
}

CollisionBox::CollisionBox(Vector3d position, Vector3d halfLengths) {
	this->position = position;
	this->size = halfLengths;
}

CollisionBox::~CollisionBox() {

}

Vector3d CollisionBox::getPosition() {
	return this->position;
}

double CollisionBox::getX() {
	return this->position.getX();
}
double CollisionBox::getY() {
	return this->position.getY();
}
double CollisionBox::getZ() {
	return this->position.getZ();
}

Vector3d CollisionBox::getSize() {
	return this->size;
}

GLboolean CollisionBox::CheckCollision(CollisionBox one, CollisionBox two) {
	// collision x-axis?
	bool collisionX = one.getX() - one.getSize().getX() <= two.getX() + two.getSize().getX() &&
		one.getX() + one.getSize().getX() >= two.getX() - two.getSize().getX();
	// collision y-axis?
	bool collisionY = one.getY() - one.getSize().getY() <= two.getY() + two.getSize().getY() &&
		one.getY() + one.getSize().getY() >= two.getY() - two.getSize().getY();
	// collision z-axis?
	bool collisionZ = one.getZ() - one.getSize().getZ() <= two.getZ() + two.getSize().getZ() &&
		one.getZ() + one.getSize().getZ() >= two.getZ() - two.getSize().getZ();
	// collision only if on all axes
	return collisionX && collisionY && collisionZ;
}

void CollisionBox::debugDrawBox() {
	// might be wrong sometimes (only visually, does not use actual position)
	glBegin(GL_QUADS);
	// front face
	glVertex3f(-size.getX(), -size.getY(), size.getZ());
	glVertex3f(size.getX(), -size.getY(), size.getZ());
	glVertex3f(size.getX(), size.getY(), size.getZ());
	glVertex3f(-size.getX(), size.getY(), size.getZ());
	// back face
	glVertex3f(-size.getX(), -size.getY(), -size.getZ());
	glVertex3f(-size.getX(), size.getY(), -size.getZ());
	glVertex3f(size.getX(), size.getY(), -size.getZ());
	glVertex3f(size.getX(), -size.getY(), -size.getZ());
	// top face
	glVertex3f(-size.getX(), size.getY(), -size.getZ());
	glVertex3f(-size.getX(), size.getY(), size.getZ());
	glVertex3f(size.getX(), size.getY(), size.getZ());
	glVertex3f(size.getX(), size.getY(), -size.getZ());
	// bottom face
	glVertex3f(-size.getX(), -size.getY(), -size.getZ());
	glVertex3f(size.getX(), -size.getY(), -size.getZ());
	glVertex3f(size.getX(), -size.getY(), size.getZ());
	glVertex3f(-size.getX(), -size.getY(), size.getZ());
	// right face
	glVertex3f(size.getX(), -size.getY(), -size.getZ());
	glVertex3f(size.getX(), size.getY(), -size.getZ());
	glVertex3f(size.getX(), size.getY(), size.getZ());
	glVertex3f(size.getX(), -size.getY(), size.getZ());
	// left face
	glVertex3f(-size.getX(), -size.getY(), -size.getZ());
	glVertex3f(-size.getX(), -size.getY(), size.getZ());
	glVertex3f(-size.getX(), size.getY(), size.getZ());
	glVertex3f(-size.getX(), size.getY(), -size.getZ());
	glEnd();
}