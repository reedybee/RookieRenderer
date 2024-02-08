#version 460 core

in vec3 fragPos;
in vec3 normal;

out vec4 fragColour;

void main() {
	fragColour = vec4(normal, 1.0f);
}