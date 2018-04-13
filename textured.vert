#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvIn;
layout(location = 3) in vec4 colorIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform mat4 normalMatrix;


out vec4 color;
out vec2 uv;

out vec3 vertexInEyeSpace;
out vec3 normalInEyeSpace;

void main(){
  // Output position of the vertex, in clip space : MVP * position
  gl_Position = projection * view * model * vec4(vertex, 1.0f);
  color = colorIn;
  uv = uvIn;
  vertexInEyeSpace = (view * model * vec4(vertex, 1)).xyz; // pos
  normalInEyeSpace = (normalMatrix * vec4(normal, 0)).xyz; // norm
}