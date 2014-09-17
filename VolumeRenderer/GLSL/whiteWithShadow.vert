#version 130

precision highp float;

in vec3 inPositions;
in vec3 inNormals;
in vec2 inUV;

//in vec3 inColors;
out vec2 fragUV;
out vec3 fragNormal;
out vec4 ShadowCoord;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 NormalMat;
uniform vec3 EyePos;
uniform mat4 DepthMVP;

uniform mat4 BiasMat;

void main()
{
	fragUV = inUV;
	fragNormal = (NormalMat * vec4(inNormals, 0.0)).xyz;

	mat4 MVP = Proj * View * Model;

	gl_Position = MVP * vec4(inPositions, 1.0);

	mat4 BiasMVP = BiasMat * DepthMVP;

	ShadowCoord = BiasMVP * vec4(inPositions, 1.0);
}

