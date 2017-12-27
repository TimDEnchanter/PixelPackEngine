//VERTEX SHADER

#version 330

layout(location = 0) in vec3 vert;
//layout(location = 1) in vec3 norm;
//layout(location = 2) in vec4 vertexColor;

uniform mat4 MVP;

out vec3 fragColor;

void main()
{
	gl_Position = MVP * vec4(vert, 1.0);

	fragColor = vert;
}