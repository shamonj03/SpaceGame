#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl\glew.h>
#include <string>

class Texture {
public:
	GLuint texture;
	GLuint shader;

	Texture(GLuint shader, std::string path);
	~Texture();

	void bind();
};
#endif TEXTURE_H