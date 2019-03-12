//VERTEX SHADER

#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

uniform mat4 Model, View, Projection;

out vec3 vertPos;
out vec3 vertNorm;

void main()
{
	gl_Position = Projection * View * Model * vec4(pos, 1.0);

	//convert position and normal to worldspace
	vertPos = vec3( Model * vec4(pos, 1.0) );
	//vertNorm = vec3( Model * vec4( (mat3(transpose(inverse(Model))) *  norm), 0.0) );
	vertNorm = vec3( Model * vec4(norm, 0.0) );
}