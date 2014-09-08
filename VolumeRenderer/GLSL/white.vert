#version 130

precision highp float;

in vec3 inPositions;
in vec3 inNormals;

//in vec3 inColors;
out vec3 fragNormal;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 NormalMat;
uniform vec3 EyePos;

void main()
{
	fragNormal = (NormalMat * vec4(inNormals, 0.0)).xyz;

	vec4 vertexInEye = View * Model * vec4(inPositions, 1.0);

	gl_Position = Proj * vertexInEye;
}

