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

class Camera {

private:

	float horizontalAngle;
	float verticalAngle;
	float fov = 90.0f;
	float distance = 16.0f;

	float perspective = 16.0f / 9.0f;

	float speed = 15.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	void updateView();

	void updateMouseCords();

public:
	glm::vec3 position;
	glm::mat4 projection, view, model;
	bool enabled;

	Camera();

	glm::vec3 getEye();
	void mouseMotion(SDL_MouseMotionEvent& e);
	void onKeyDown(const Uint8 *keyboard_state_array, float dt);
};

inline Camera::Camera() : position(glm::vec3(0.0f, 0.0f, 0.0f)), horizontalAngle(0.0f), verticalAngle(0.0f), model(glm::mat4(1.0f)), enabled(true) {
	updateView();
}

inline void Camera::updateView() {
	projection = glm::perspective(glm::radians(fov), perspective, 0.1f, 100.0f);
	view = glm::lookAt(getEye(), position, glm::vec3(0, 1, 0));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(&projection[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(&view[0][0]);
}

inline void Camera::mouseMotion(SDL_MouseMotionEvent& e) {
	updateMouseCords();
	updateView();
}


inline void Camera::onKeyDown(const Uint8 *keyboard_state_array, float dt) {
	if (keyboard_state_array[SDL_SCANCODE_W]) {
		position = position - glm::vec3(sin(horizontalAngle)*cos(verticalAngle), sin(verticalAngle), cos(horizontalAngle)*cos(verticalAngle)) * speed * dt;
		updateView();
	}
	else if (keyboard_state_array[SDL_SCANCODE_S]) {
		position = position + glm::vec3(sin(horizontalAngle)*cos(verticalAngle), sin(verticalAngle), cos(horizontalAngle)*cos(verticalAngle)) * speed * dt;
		updateView();
	}

	if (keyboard_state_array[SDL_SCANCODE_A]) {
		horizontalAngle += glm::radians(15.0f * speed * dt);
		updateView();
	}
	else if (keyboard_state_array[SDL_SCANCODE_D]) {
		horizontalAngle -= glm::radians(15.0f * speed * dt);
		updateView();
	}
}

inline void Camera::updateMouseCords() {
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	horizontalAngle -= mouseSpeed * x;
	verticalAngle += mouseSpeed * y;

	if (verticalAngle < -M_PI / 2 + 0.001)
		verticalAngle = -M_PI / 2 + 0.001;
	if (verticalAngle > M_PI / 2 - 0.001)
		verticalAngle = M_PI / 2 - 0.001;
}

inline glm::vec3 Camera::getEye() {
	return position + distance * glm::vec3(sin(horizontalAngle)*cos(verticalAngle), sin(verticalAngle), cos(horizontalAngle)*cos(verticalAngle));
}