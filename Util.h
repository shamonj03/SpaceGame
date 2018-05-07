#ifndef UTIL_H
#define UTIL_H

#include <glm\gtc\matrix_transform.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <vector>

class Util {
public:
	static glm::vec3 rotate(glm::vec3& vec, float angle);

	static void rotate(glm::vec3* verticies, int length, float angle, glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f));
	static void rotate(std::vector<glm::vec3>& verticies, int length, float angle, glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f));

	static float randf();

	static glm::vec3 limit(const glm::vec3 &v, float maximum);
};

#endif // UTIL_H