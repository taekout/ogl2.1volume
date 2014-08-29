#version 130

precision highp float;

in vec3 inPositions;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;
//uniform mat4 NormalMat;
//uniform vec3 LightPos;
//uniform vec3 EyePos;

void main()
{
	gl_Position = Proj * View * Model * vec4(inPositions, 1.0);
}

