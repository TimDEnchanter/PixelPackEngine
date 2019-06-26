//VERTEX SHADER

#version 330

layout(location = 0) in vec2 pos;
layout(location = 2) in vec2 uv;

uniform mat4 Projection;

out vec2 vertUV;

void main()
{
	gl_Position = Projection * vec4(pos, 0.0, 1.0);
	vertUV = uv;
}