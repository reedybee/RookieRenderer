#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 norm;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0);
	fragPos = vec3(model * vec4(pos, 1.0));
	normal = norm;
	//normal = mat3(transpose(inverse(model))) * aNormal;
	//texCoord = vec2(aTexCoord); 
}