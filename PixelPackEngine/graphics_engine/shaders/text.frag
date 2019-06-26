//FRAGMENT SHADER

#version 330

uniform sampler2D image;
uniform vec3 textColor;

in vec2 vertUV;

out vec4 fragColor;

void main()
{
	vec4 sample = vec4(1.0, 1.0, 1.0, texture(image, vertUV).a);
	fragColor = vec4(textColor, 1.0) * sample;
}