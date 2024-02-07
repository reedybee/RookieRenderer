#version 460 core

in vec3 fragPos;

out vec4 fragColour;

void main() {
	fragColour = vec4(fragPos,1.0f);
}