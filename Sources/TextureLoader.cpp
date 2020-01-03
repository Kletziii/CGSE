#include <iostream>
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>

#include "../Headers/TextureLoader.h"

using namespace std;

extern "C" {
#include "../Headers/TgaLoader.h"
}

TextureLoader::TextureLoader() {

}

TextureLoader::~TextureLoader() {

}

void TextureLoader::initialize() {
	loadTexture("Resources/crate.tga", &textures["crate"]);
	loadTexture("Resources/flagstone.tga", &textures["flagstone"]);
	loadTexture("Resources/fire.tga", &textures["fire"]);
	loadTexture("Resources/bricks.tga", &textures["bricks"]);
	loadTexture("Resources/coin_front.tga", &textures["coin_front"]);
	loadTexture("Resources/coin_core.tga", &textures["coin_core"]);
	loadTexture("Resources/coin_back.tga", &textures["coin_back"]);
	loadTexture("Resources/metalcrate.tga", &textures["metalcrate"]);
	loadTexture("Resources/lava.tga", &textures["lava"]);
}

GLuint TextureLoader::get(string textureName) {
	return textures[textureName];
}

void TextureLoader::loadTexture(string filename, GLuint* texture) {
	GLsizei w, h;
	tgaInfo *info = 0;
	int mode;

	info = tgaLoad(&filename[0]);

	if (info->status != TGA_OK) {
		fprintf(stderr, "error loading texture image: %d\n", info->status);

		return;
	}
	if (info->width != info->height) {
		fprintf(stderr, "Image size %d x %d is not rectangular, giving up.\n",
			info->width, info->height);
		return;
	}

	mode = info->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
	glGenTextures(1, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Upload the texture bitmap. 
	w = info->width;
	h = info->height;

	reportGLError("before uploading texture");
	GLint format = (mode == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format,
		GL_UNSIGNED_BYTE, info->imageData);
	reportGLError("after uploading texture");

	tgaDestroy(info);
}

void TextureLoader::reportGLError(const char * msg)
{
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error: %s %s\n", msg, errString);
	}
	return;
}