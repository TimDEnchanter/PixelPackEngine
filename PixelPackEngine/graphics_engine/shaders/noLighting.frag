//FRAGMENT SHADER

#version 330

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

out vec4 fragColor;

void main()
{
	fragColor = vec4(material.diffuse, 1.0);
}