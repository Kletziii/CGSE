#include <stdlib.h> // for exit (must be above GL)
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/LOD.h"

LOD::LOD() {
	// needed
}

LOD::LOD(Vector3d position, Vector3d halfLengths, bool destructible, bool harmful, float textureFactor, GLuint texture) {
	this->position = position;
	this->halfLengths = halfLengths;
	this->texture = texture;
	this->textureFactor = textureFactor;
	this->hit = false;
	this->destructible = destructible;
	this->harmful = harmful;
	this->collisionBox = CollisionBox(position, halfLengths);

	drawLOD();
}

LOD::~LOD() {

}

void LOD::drawLOD() {
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(position.getX(), position.getY(), position.getZ());

	glBegin(GL_QUADS);
	// front face
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLengths.getX(), -halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLengths.getX(), -halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLengths.getX(), halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLengths.getX(), halfLengths.getY(), halfLengths.getZ());

	// back face
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfLengths.getX(), -halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfLengths.getX(), halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfLengths.getX(), halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfLengths.getX(), -halfLengths.getY(), -halfLengths.getZ());

	// top face (tex coords adapted for lava lake)
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, textureFactor * halfLengths.getZ());
	glVertex3f(-halfLengths.getX(), halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-halfLengths.getX(), halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(textureFactor * halfLengths.getX(), 0.0f);
	glVertex3f(halfLengths.getX(), halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(textureFactor * halfLengths.getX(), textureFactor * halfLengths.getZ());
	glVertex3f(halfLengths.getX(), halfLengths.getY(), -halfLengths.getZ());

	// bottom face
	glNormal3f(0.0, -1.0, 1.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfLengths.getX(), -halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfLengths.getX(), -halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfLengths.getX(), -halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfLengths.getX(), -halfLengths.getY(), halfLengths.getZ());
	// right face
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLengths.getX(), -halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLengths.getX(), halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfLengths.getX(), halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfLengths.getX(), -halfLengths.getY(), halfLengths.getZ());
	// left face
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLengths.getX(), -halfLengths.getY(), -halfLengths.getZ());
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfLengths.getX(), -halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfLengths.getX(), halfLengths.getY(), halfLengths.getZ());
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLengths.getX(), halfLengths.getY(), -halfLengths.getZ());
	glEnd();

	glPopMatrix();
}

Vector3d LOD::getPosition() const {
	return this->position;
}

void LOD::setPosition(Vector3d newPosition) {
	this->position = newPosition;
	this->collisionBox = CollisionBox(position, halfLengths);
}

void LOD::setCollision() {
	this->hit = true;
}

bool LOD::isHit() const {
	return this->hit;
}

bool LOD::isDestructible() const {
	return this->destructible;
}

bool LOD::isHarmful() const {
	return this->harmful;
}

CollisionBox LOD::getCollisionBox() const {
	return this->collisionBox;
}