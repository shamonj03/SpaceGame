#ifndef SHADER_H
#define SHADER_H

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <string>

class Shader {
public:
	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	static void setUniform(GLuint program, std::string name, glm::mat4 m);

	static void setUniform(GLuint shader, std::string name, glm::vec3 vec);

	static void setUniform(GLuint shader, std::string name, glm::vec4 vec);

	static void setUniform(GLuint shader, std::string name, float num);

	static void setUniform(GLuint shader, std::string name, float one, float two, float three);

	static void setUniform(GLuint shader, std::string name, float one, float two, float three, float four);

	static void bindArray(GLenum target, GLuint& buffer, int size, GLvoid * data, GLenum mode);

	static void sendArray(int index, int len, GLuint& buffer, int size = -1, GLvoid * data = NULL, GLenum mode = GL_STATIC_DRAW);
};
#endif // SHADER_H