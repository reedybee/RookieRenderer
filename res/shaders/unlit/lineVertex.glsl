#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 textureCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

	fragPos = vec3(model * vec4(pos, 1.0));

	gl_Position = projection * view * vec4(fragPos, 1.0);
}