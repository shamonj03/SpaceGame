#ifndef UTIL_HPP
#define UTIL_HPP

#include <glm\gtc\matrix_transform.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class Util {
public:
	static glm::vec3 rotate(glm::vec3& vec, float angle);

	static void rotate(glm::vec3* verticies, int length, float angle, glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f));

	static float randf();

	static glm::vec3 limit(const glm::vec3 &v, float maximum);
};

inline glm::vec3 Util::rotate(glm::vec3& vec, float angle) {
	glm::mat4 rot_mat = glm::rotate(glm::mat4x4(1), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	vec = rot_mat * glm::vec4(vec, 0);
	return vec;
}

inline void Util::rotate(glm::vec3* verticies, int length, float angle, glm::vec3 axis) {
	glm::mat4 rot_mat = glm::rotate(glm::mat4x4(1), glm::radians(angle), axis);
	for (int i = 0; i < length; i++) {
		verticies[i] = rot_mat * glm::vec4(verticies[i], 0);
	}
}

inline float Util::randf() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline glm::vec3 Util::limit(const glm::vec3 &vec, float maximum) {
	auto length = glm::length(vec);
	if (length > maximum) {
		return glm::normalize(vec) * maximum;
	}
	return vec;
}

#endif // UTIL_HPP