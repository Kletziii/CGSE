#include <stdlib.h> // for exit (must be above GL)
#include <cmath>
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/Coin.h"

#ifndef M_PI
#define M_PI 3.141592653
#endif

#define RAD(x) (((x)*M_PI)/180.)

Coin::Coin(Vector3d position, GLuint frontTexture, GLuint coreTexture, GLuint backTexture)
	: Coin(position, frontTexture, coreTexture, backTexture, 0.3f, 0.1f, 12) {
}

Coin::Coin(Vector3d position, GLuint frontTexture, GLuint coreTexture, GLuint backTexture, float radius, float width, int precision) {
	this->position = position;
	this->frontTexture = frontTexture;
	this->coreTexture = coreTexture;
	this->backTexture = backTexture;
	this->radius = radius;
	this->width = width;
	this->precision = precision;
	this->angle = 0;
	this->collisionBox = CollisionBox(position, radius);
}

Coin::~Coin() {

}

void Coin::updateRotation() {
	glPushMatrix();

	// increase angle
	angle += 0.5f;
	if (angle >= 360) angle -= 360;

	// center coin at given position
	glTranslatef(position.getX(), position.getY(), position.getZ() - width / 2);

	// rotate around center
	glTranslatef(0, 0, width / 2);
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -width / 2);

	// draw cylinder
	GLUquadricObj *coin = gluNewQuadric();
	gluQuadricDrawStyle(coin, GLU_FILL);
	gluQuadricTexture(coin, true);
	gluQuadricNormals(coin, GLU_SMOOTH);
	glBindTexture(GL_TEXTURE_2D, coreTexture);
	gluCylinder(coin, radius, radius, width, 2 * precision, 2 * precision);

	// draw circles (number of sides is twice the value of precision)
	glBindTexture(GL_TEXTURE_2D, frontTexture);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * M_PI; i += M_PI / precision) {
		float xcos = cosf(i);
		float ysin = sinf(i);
		float x = xcos * radius;
		float y = ysin * radius;
		float tx = xcos * 0.5 + 0.5;
		float ty = ysin * 0.5 + 0.5;

		glTexCoord2f(-tx, ty);
		glNormal3f(0, 0, -1);
		glVertex3f(x, y, 0.0);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, backTexture);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * M_PI; i += M_PI / precision) {
		float xcos = cosf(i);
		float ysin = sinf(i);
		float x = xcos * radius;
		float y = ysin * radius;
		float tx = xcos * 0.5 + 0.5;
		float ty = ysin * 0.5 + 0.5;

		glTexCoord2f(tx, ty);
		glNormal3f(0, 0, 1);
		glVertex3f(x, y, width);
	}
	glEnd();

	glPopMatrix();
}

CollisionBox Coin::getCollisionBox() {
	return this->collisionBox;
}