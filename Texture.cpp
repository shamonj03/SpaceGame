#include "Texture.h"
#include <SDL_image.h>
#include <iostream>
Texture::Texture(GLuint shader, std::string path) {
	// load sample.png into image
	SDL_Surface *image = IMG_Load(path.c_str());
	if (image == NULL) {
		printf("Failed to open texture\n");
	}

	int mode = GL_RGB;

	if (image->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(image); 
	glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::~Texture()
{
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, "tex"), 0);
}