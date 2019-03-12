//VERTEX SHADER

#version 330

layout(location = 0) in vec3 vert;

uniform mat4 Model, View, Projection;

out vec3 vertColor;

void main()
{
	gl_Position = Projection * View * Model * vec4(vert, 1.0);
}