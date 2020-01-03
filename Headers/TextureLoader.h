#pragma once

#include <iostream>
#include <map>

class TextureLoader {
public:
	TextureLoader();
	~TextureLoader();
	void initialize();
	GLuint get(std::string textureName);
private:
	void reportGLError(const char * msg);
	void loadTexture(std::string filename, GLuint* texture);
	std::map<std::string, GLuint> textures;
};