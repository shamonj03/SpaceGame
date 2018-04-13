#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform vec3 lightPosition;
uniform mat4 modelViewMatrix;

uniform vec4 Ia;
uniform vec4 Is;
uniform float s;


in vec4 color;
in vec3 vertexInEyeSpace;
in vec3 normalInEyeSpace;

out vec4 colorOut;

void main(){
    vec3 lightInEyeSpace = (view * model * vec4(lightPosition, 1)).xyz; // pos
	vec3 lightDir = normalize(lightInEyeSpace - vertexInEyeSpace);
	vec3 normal = normalize(normalInEyeSpace);
	vec3 viewDir = normalize(-vertexInEyeSpace);

	vec3 h = normalize(viewDir + lightDir);
	
	// Ambient
	vec4 ambient = Ia * color;

	// Diffuse
	
	// Specular
	float NdotH = dot(normal, h); 
	float intensity = pow(max(NdotH, 0), s);
	vec4 specular =  Is * intensity;
	
	// Color
	colorOut = ambient + specular;
}