#include <stdlib.h> // for exit (must be above GL)

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../Headers/Light.h"

Light::Light(GLenum lightID, GLfloat position[], GLfloat ambient[], GLfloat diffuse[], GLfloat specular[]) {
	this->m_LightID = lightID;
	
	glLightfv(this->m_LightID, GL_DIFFUSE, diffuse);
	glLightfv(this->m_LightID, GL_SPECULAR, specular);
	glLightfv(this->m_LightID, GL_AMBIENT, ambient);
	glLightfv(this->m_LightID, GL_POSITION, position);
}

Light::~Light() {
}

void Light::ActivateLight() {
	glEnable(this->m_LightID);
}

void Light::DeactivateLight() {
	glDisable(this->m_LightID);
}
