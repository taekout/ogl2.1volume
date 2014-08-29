#version 130

precision highp float;

in vec3 inPositions;
in vec3 inNormals;

uniform mat4 MVP;
uniform mat4 depthBiasMVP;
//uniform mat4 NormalMat;
//uniform vec3 LightPos;
//uniform vec3 EyePos;

void main()
{
	gl_Position = Proj * View * Model * vec4(inPositions, 1.0);
}

