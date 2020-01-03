#include <stdlib.h> // for exit (must be above GL)
#include <cmath>
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/Wall.h"

Wall::Wall(float beginX, float beginZ, float endX, float endZ, float height, bool negateNormal, float textureFactor, GLuint texture) {
	this->beginPosition = Vector3d(beginX, 0, beginZ);
	this->endPosition = Vector3d(endX, height, endZ);
	this->textureFactor = textureFactor;
	this->texture = texture;
	this->collisionBox = CollisionBox(Vector3d((beginX + endX) / 2, height / 2, (beginZ + endZ) / 2), Vector3d(abs(endX - beginX) / 2, height / 2, abs(endZ - beginZ) / 2));

	calculateNormal(negateNormal);
}


Wall::~Wall() {

}

void Wall::calculateNormal(bool negateNormal) {
	Vector3d v1 = Vector3d(beginPosition.getX(), endPosition.getY(), beginPosition.getZ());
	Vector3d v2 = Vector3d(endPosition.getX(), endPosition.getY(), endPosition.getZ());
	Vector3d v3 = Vector3d(beginPosition.getX(), beginPosition.getY(), beginPosition.getZ());
	//Vector3d v4 = Vector3d(endPosition.getX(), beginPosition.getY(), endPosition.getZ()); // not needed for normal

	// calculate normal (at v1)
	normal = (v2 - v1).cross((v3 - v1)).normalized();

	// look at wall from the other side
	if (negateNormal) {
		normal = normal.neg();
	}
}

void Wall::draw() {
	// debug normals
	//Vector3d v1 = Vector3d(beginPosition.getX(), endPosition.getY(), beginPosition.getZ());
	//Vector3d debugNormal = v1 + normal;
	//glPushMatrix();
	//glTranslatef(debugNormal.getX(), debugNormal.getY(), debugNormal.getZ());
	//glutSolidSphere(0.25, 15, 15);
	//glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glNormal3f(normal.getX(), normal.getY(), normal.getZ());

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(beginPosition.getX(), beginPosition.getY(), beginPosition.getZ()); // v3

	glTexCoord2f(0.0f, textureFactor * abs(endPosition.getY() - beginPosition.getY()));
	glVertex3f(beginPosition.getX(), endPosition.getY(), beginPosition.getZ()); // v1

	glTexCoord2f(textureFactor * sqrt(pow(abs((beginPosition.getX() - endPosition.getX())), 2) + pow(abs((beginPosition.getZ() - endPosition.getZ())), 2)), textureFactor * abs(endPosition.getY() - beginPosition.getY()));
	glVertex3f(endPosition.getX(), endPosition.getY(), endPosition.getZ()); // v2

	glTexCoord2f(textureFactor * sqrt(pow(abs((beginPosition.getX() - endPosition.getX())), 2) + pow(abs((beginPosition.getZ() - endPosition.getZ())), 2)), 0.0f);
	glVertex3f(endPosition.getX(), beginPosition.getY(), endPosition.getZ()); // v4
	glEnd();
	glPopMatrix();
}

CollisionBox Wall::getCollisionBox() {
	return this->collisionBox;
}