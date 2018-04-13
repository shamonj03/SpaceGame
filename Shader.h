#pragma once

#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
public:
	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << VertexShaderStream.rdbuf();
			VertexShaderCode = sstr.str();
			VertexShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
			getchar();
			return 0;
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << FragmentShaderStream.rdbuf();
			FragmentShaderCode = sstr.str();
			FragmentShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragment_file_path);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}

		// Link the program
		printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}

	static inline void setUniform(GLuint program, std::string name, glm::mat4 m) {
		GLint uniform = glGetUniformLocation(program, name.c_str());
		glUniformMatrix4fv(uniform, 1, GL_FALSE, &m[0][0]);
	}

	static inline void setUniform(GLuint shader, std::string name, glm::vec3 vec) {
		GLint uniform = glGetUniformLocation(shader, name.c_str());
		glUniform3f(uniform, vec[0], vec[1], vec[2]);
	}

	static inline void setUniform(GLuint shader, std::string name, glm::vec4 vec) {
		GLint uniform = glGetUniformLocation(shader, name.c_str());
		glUniform4f(uniform, vec[0], vec[1], vec[2], vec[3]);
	}

	static inline void setUniform(GLuint shader, std::string name, float num) {
		GLint uniform = glGetUniformLocation(shader, name.c_str());
		glUniform1f(uniform, num);
	}


	static inline void setUniform(GLuint shader, std::string name, float one, float two, float three) {
		GLint uniform = glGetUniformLocation(shader, name.c_str());
		glUniform3f(uniform, one, two, three);
	}
	static inline void setUniform(GLuint shader, std::string name, float one, float two, float three, float four) {
		GLint uniform = glGetUniformLocation(shader, name.c_str());
		glUniform4f(uniform, one, two, three, four);
	}

	static inline void bindArray(GLenum target, GLuint& buffer, int size, GLvoid * data, GLenum mode) {
		glGenBuffers(1, &buffer);
		glBindBuffer(target, buffer);
		glBufferData(target, size, data, mode);
	}

	static inline void sendArray(int index, int len, GLuint& buffer, int size = -1, GLvoid * data = NULL, GLenum mode = GL_STATIC_DRAW) {
		glEnableVertexAttribArray(index);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		if (mode != GL_STATIC_DRAW) {
			glBufferData(GL_ARRAY_BUFFER, size, NULL, mode);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		}
		glVertexAttribPointer(index, len, GL_FLOAT, GL_FALSE, 0, (void*)0);

	}
};