#include <cstdio>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Engine.h"
#include "Camera.hpp"
#include "Config.h"

#include "Shader.h"
#include "Constants.h"

Engine::Engine(float width_, float height_) : 
    width(width_),
	height(height_) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Failed to initialize SDL Error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	window = SDL_CreateWindow("Particles SDL/GLEW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_OPENGL);

	if (window == NULL) {
		std::cout << "Failed to initialize SDL window Error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		std::cout << "Failed to initialize SDL context Error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	glewExperimental = GL_TRUE;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);
	SDL_WarpMouseInWindow(window, height_ / 2, height_ / 2);

	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	standardShader = Shader::LoadShaders("standard.vert", "standard.frag");
	texturedShader = Shader::LoadShaders("textured.vert", "textured.frag");
	camera = new Camera();
}

void Engine::draw(float dt) {
	// Draw Standard
	glUseProgram(standardShader);
	Shader::setUniform(standardShader, "model", camera->model);
	Shader::setUniform(standardShader, "view", camera->view);
	Shader::setUniform(standardShader, "projection", camera->projection);
	Shader::setUniform(standardShader, "normalMatrix", glm::inverse(glm::transpose(camera->model)));
	Shader::setUniform(standardShader, "cameraRight", camera->view[0][0], camera->view[1][0], camera->view[2][0]);
	Shader::setUniform(standardShader, "cameraUp", camera->view[0][1], camera->view[1][1], camera->view[2][1]);
	Shader::setUniform(standardShader, "lightPosition", camera->getEye());
	Shader::setUniform(standardShader, "Ia", Config::Ia);
	Shader::setUniform(standardShader, "Is", Config::Is);
	Shader::setUniform(standardShader, "s", Config::s);
	drawStandard(dt);

	// Draw Textured
	glEnable(GL_TEXTURE_2D);
	glUseProgram(texturedShader);
	Shader::setUniform(texturedShader, "model", camera->model);
	Shader::setUniform(texturedShader, "view", camera->view);
	Shader::setUniform(texturedShader, "projection", camera->projection);
	Shader::setUniform(standardShader, "normalMatrix", glm::inverse(glm::transpose(camera->model)));
	Shader::setUniform(texturedShader, "cameraRight", camera->view[0][0], camera->view[1][0], camera->view[2][0]);
	Shader::setUniform(texturedShader, "cameraUp", camera->view[0][1], camera->view[1][1], camera->view[2][1]);
	Shader::setUniform(standardShader, "lightPosition", camera->getEye());

	Shader::setUniform(standardShader, "Ia", Config::Ia);
	Shader::setUniform(standardShader, "Is", Config::Is);

	Shader::setUniform(standardShader, "s", Config::s);
	drawTextured(dt);
	glDisable(GL_TEXTURE_2D);
}

void Engine::handleInput() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			onKeyDown(event.key);
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}

			if (event.key.keysym.sym == SDLK_BACKQUOTE) { // Tilda ~
				paused = !paused;
			}
			onKeyUp(event.key);
			break;
		case SDL_MOUSEMOTION:
			if (camera->enabled) {
				camera->mouseMotion(event.motion);
			}
			onMouseMotion(event.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event.button);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event.button);
			break;
		}
	}
}

void Engine::start() {
	SDL_Event event;
	int lastFrameTime = 0;
	float dt = DELTA_TIME;
	int last = SDL_GetTicks();

	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

	int frames = 0;
	while (!quit) {
		handleInput();

		int millisPerFrame = (int)(1000 * dt);
		int now = SDL_GetTicks();
		int nextFrameTime = lastFrameTime + millisPerFrame;
		lastFrameTime = now;

		if (nextFrameTime > now) {
			SDL_Delay(nextFrameTime - now);
		}

		if (camera->enabled) {
			camera->onKeyDown(keyboard_state_array, dt);
		}

		if (paused) {
			update(dt);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		draw(dt);

		frames++;

		if (SDL_GetTicks() - last >= 1000) {
			std::cout << "Fps: " << frames << std::endl;
			char c[36];
			sprintf_s(c, "SDL/GLEW - FPS: %i", frames);
			SDL_SetWindowTitle(window, c);
			frames = 0;
			last = SDL_GetTicks();
		}

		SDL_GL_SwapWindow(window); //Double buffering
	}
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

GLuint Engine::loadTexture(std::string bmpFile) {
	SDL_Surface *surface = SDL_LoadBMP(bmpFile.c_str());
	if (surface == NULL) {
		printf("Failed to open texture\n");
		return NULL;
	}
	SDL_LockSurface(surface);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(surface);
	return texture;
}