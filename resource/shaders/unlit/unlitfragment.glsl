#version 460 core

in vec3 fragPos;

out vec4 fragColour;

void main() {
	if (distance(fragPos, vec3(0)) < 0.5) {
		discard;
	}
	fragColour = vec4(1.0f);
}