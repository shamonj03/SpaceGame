#include "Player.h"
#include "World.h"
#include <GL/glew.h>
#include "Shader.h"

#include "ParticleSystem.h"
#include "StandardEmitter.hpp"
#include "BulletEmitter.hpp"
#include "Ship.h"
#include "Util.h"

Player::Player(World* world_, GLfloat shader_, glm::vec3 position_) : Ship(world_, shader_, position_) {
}

Player::~Player() {
}


void Player::update(float dt) {
	Ship::update(dt);

	// Slows down if key released.
	if (released) {
		velocity *= 0.95f;
	}
}

void Player::onKeyDown(SDL_KeyboardEvent& e) {
	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
	if (keyboard_state_array[SDL_SCANCODE_W]) { // Accel forward
		acceleration += glm::vec3(0, 0.75f, 0);
		released = false;
	}
	if (keyboard_state_array[SDL_SCANCODE_S]) { // Deaccel
		acceleration -= glm::vec3(0, 0.75f, 0);
		released = false;
	}

	if (keyboard_state_array[SDL_SCANCODE_A]) { // Rotate Left
		angle += 5.0f;
	}
	if (keyboard_state_array[SDL_SCANCODE_D]) { // Rotate Right
		angle -= 5.0f;
	}
	if (keyboard_state_array[SDL_SCANCODE_SPACE]) { // Rotate Right
		addEmitter(new BulletEmitter(this), shader);
	}
}


void Player::onMouseButtonDown(SDL_MouseButtonEvent& e) {
	if (e.button == SDL_BUTTON_LEFT) { // FIRE THE LASER!
		addEmitter(new BulletEmitter(this), shader);
	}
}

void Player::onKeyUp(SDL_KeyboardEvent& e) {
	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

	if (e.keysym.sym == SDLK_w || e.keysym.sym == SDLK_s) { // Accel forward
		acceleration = glm::vec3(0, 0, 0);
		released = true;
	}

	if (e.keysym.sym == SDLK_SPACE) { // Stop the laser :C

	}
}