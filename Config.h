#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include <string>
#include <glm/glm.hpp>

class Config {
public:

	// Lighting
	static glm::vec4 const Ia;
	static glm::vec4 const Is;

	// Material parameters
	static float const s;
};


#endif // !_CONFIG_HPP