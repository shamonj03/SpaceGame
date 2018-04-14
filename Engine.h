#pragma once

#include <GL/glew.h>
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstdio>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Camera.hpp"
#include "Config.h"

using namespace std;

class Engine {
protected:
	bool quit, paused;
	float width, height;

	GLuint standardShader, texturedShader;
	Camera* camera;

private:
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event event;

	void handleInput();

	virtual void update(float dt) = 0;
	virtual void draw(float dt);

	virtual void drawStandard(float dt) = 0;
	virtual void drawTextured(float dt) = 0;

	virtual void onKeyDown(SDL_KeyboardEvent& e) {}
	virtual void onKeyUp(SDL_KeyboardEvent& e) {}
	virtual void onMouseMotion(SDL_MouseMotionEvent& e) {}
	virtual void onMouseButtonDown(SDL_MouseButtonEvent& e) {}
	virtual void onMouseButtonUp(SDL_MouseButtonEvent& e) {}

public:
	Engine(float width_, float height_);
	~Engine() {}

	void start();

	virtual GLuint loadTexture(std::string bmpFile);
};
