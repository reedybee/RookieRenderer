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
	// textures
	vec4 albedo = texture(albedoMap, texCoord);

	// ambient 
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * vec3(1.0);

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPosition - fragPos);
	float diff  = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0);

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * vec3(1.0);
	
	vec3 result = (ambient + diffuse + specular) * vec3(albedo) * objectColour;

	fragColour = vec4(result, 1.0);
	//fragColour = vec4(fragPos, 1.0);
}