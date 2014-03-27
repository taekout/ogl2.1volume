#version 130

precision highp float;

in vec3 in_Position;
out vec3 fragColor;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	fragColor = vec3(1.0, 0.0, 0.0);

	vec4 vertex = vec4(in_Position, 1.0);
	gl_Position = vec4(Proj * View * Model * vertex);
}