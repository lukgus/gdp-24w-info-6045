#version 400

layout(location=0) in vec4 in_Position;	// v -0.215260 2.647317 0.519682
layout(location=1) in vec4 in_TexUVx2;
layout(location=2) in vec4 in_Normal;
layout(location=3) in vec4 in_BoneIds;	// b 2

out vec4 ex_Position;			
out vec4 ex_PositionWorld;
out vec4 ex_Normal;
out vec4 ex_UV_x2;

uniform mat4 ModelMatrix;	
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 BoneMatrices[5];

void main(void)
{
	mat4 boneMatrix = BoneMatrices[int(in_BoneIds[0])];
	vec4 position = boneMatrix * in_Position;

	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	gl_Position = MVP * position;
}
