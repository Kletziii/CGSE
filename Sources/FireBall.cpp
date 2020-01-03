#include <stdlib.h> // for exit (must be above GL)
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/FireBall.h"

FireBall::FireBall(Vector3d cameraPosition, Vector3d lookAt, float speed, GLuint texture) {
	this->direction = (lookAt - cameraPosition).normalized(); // calculate normalized direction vector
	this->position = cameraPosition + direction; // spawn fire ball at some distance to the player
	this->speed = speed;
	this->distanceTraveled = 0;
	this->texture = texture;
	this->hit = false;
}

FireBall::~FireBall() {

}

void FireBall::updatePosition() {
	glPushMatrix();

	// draw fire ball
	GLUquadricObj *sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, true);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(position.getX(), position.getY(), position.getZ());
	glRotatef(90, 0, 1, 0);
	gluSphere(sphere, 0.2, 15, 15);

	// update position and travelled distance
	position = position + (direction * speed);
	distanceTraveled += speed;

	// set collision box
	collisionBox = CollisionBox(position, 0.2f);
	//collisionBox.debugDrawBox(); // uncomment to make collision box visible

	// floor collision
	if (position.getY() <= 0.2) {
		setCollision();
	}

	glPopMatrix();
}

float FireBall::getDistanceTraveled() {
	return this->distanceTraveled;
}

void FireBall::setCollision() {
	this->hit = true;
}

bool FireBall::isHit() {
	return this->hit;
}

CollisionBox FireBall::getCollisionBox() {
	return this->collisionBox;
}