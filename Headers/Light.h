#pragma once

class Light {
public:
	Light(GLenum lightID, GLfloat position[], GLfloat ambient[], GLfloat diffuse[], GLfloat specular[]);
	~Light();
	void ActivateLight();
	void DeactivateLight();

private:
	GLenum m_LightID;
};