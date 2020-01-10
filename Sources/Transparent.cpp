#include <stdlib.h> // for exit (must be above GL)
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/Transparent.h"

Transparent::Transparent() {
	// needed
}

Transparent::Transparent(Vector3d position, Vector3d triangle1, Vector3d triangle2, Vector3d triangle3, Vector3d color, double alpha, float textureFactor, GLuint texture) {
	this->position = position;
	this->triangle1 = triangle1;
	this->triangle2 = triangle2;
	this->triangle3 = triangle3;
	this->color = color;
	this->texture = texture;
	this->alpha = alpha;
	this->textureFactor = textureFactor;

	calcCentroid();
}

Transparent::~Transparent() {

}

void Transparent::drawTransparent() {
	glPushMatrix();

	//glBindTexture(GL_TEXTURE_2D, texture);
	//glTranslatef(position.getX(), position.getY(), position.getZ());

	//GLfloat mat_white[] = { 1.0, 1.0, 1.0, 0.5 };
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white);
	//glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

	glBegin(GL_TRIANGLES);
	// front face
	//glNormal3f(0.0, 0.0, 1.0);
	glColor4d(color.getX(), color.getY(), color.getZ(), alpha);
	//glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(triangle1.getX(), triangle1.getY(), triangle1.getZ());
	//glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(triangle2.getX(), triangle2.getY(), triangle2.getZ());
	//glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(triangle3.getX(), triangle3.getY(), triangle3.getZ());
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLengths.getX(), halfLengths.getY(), halfLengths.getZ());
	glEnd();
	
	/*GLfloat mat_white2[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white2);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);*/

	glPopMatrix();
}

void Transparent::calcCentroid() {
	this->center = Vector3d(((triangle1.getX() + triangle2.getX() + triangle3.getX()) / 3), ((triangle1.getY() + triangle2.getY() + triangle3.getY()) / 3),
	((triangle1.getZ() + triangle2.getZ() + triangle3.getZ()) / 3));
}

Vector3d Transparent::getPosition() const {
	return this->position;
}

Vector3d Transparent::getCenter() const {
	return this->center;
}

void Transparent::setPosition(Vector3d newPosition) {
	this->position = newPosition;
}

void Transparent::setCollision() {
	this->hit = true;
}

bool Transparent::isHit() const {
	return this->hit;
}

bool Transparent::isDestructible() const {
	return this->destructible;
}

bool Transparent::isHarmful() const {
	return this->harmful;
}

CollisionBox Transparent::getCollisionBox() const {
	return this->collisionBox;
}