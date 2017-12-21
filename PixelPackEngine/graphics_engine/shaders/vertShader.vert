//VERTEX SHADER

#version 330

layout(location = 0) in vec3 vert;
//layout(location = 1) in vec3 norm;
//layout(location = 2) in vec4 vertexColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

//out vec4 fragColor;

void main()
{
	gl_Position = projection * view * model * vec4(vert, 1.0);

	//fragColor = vertexColor;
}