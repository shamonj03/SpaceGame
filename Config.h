#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include <string>
#include <glm/glm.hpp>

class Config {
public:

	static enum SIM_TYPE { BALLS, WIND };

	// Lighting
	static glm::vec4 const Ia;
	static glm::vec4 const Is;

	// Material parameters
	static float const s;

	static SIM_TYPE const type;
};


#endif // !_CONFIG_HPP