//VERTEX SHADER

#version 330

layout(location = 0) in vec3 vert;

uniform mat4 Model, View, Projection;
uniform vec3 objColor;

out vec3 fragColor;

void main()
{
	gl_Position = Projection * View * Model * vec4(vert, 1.0);

	fragColor = objColor;
}