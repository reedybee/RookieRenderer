#version 460 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 cameraPos;
uniform vec3 lightPosition;
uniform vec3 objectColour;
uniform sampler2D albedoMap;

out vec4 fragColour;

void main() {
	fragColour = vec4(1.0, 1.0, 1.0, 1.0);
}