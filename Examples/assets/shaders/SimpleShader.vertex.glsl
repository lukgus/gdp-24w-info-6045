#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_TexUVx2;
layout(location=2) in vec4 in_Normal;
layout(location=3) in vec4 in_BoneIds;
layout(location=4) in vec4 in_BoneWeights;

uniform mat4 ModelMatrix;	
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 BoneMatrices[50];

out vec4 ex_BoneId;

void main(void)
{
	ex_BoneId = in_BoneIds;
	mat4 boneTransform = BoneMatrices[int(in_BoneIds[0])] * in_BoneWeights[0];
	boneTransform += BoneMatrices[int(in_BoneIds[1])] * in_BoneWeights[1];
	boneTransform += BoneMatrices[int(in_BoneIds[2])] * in_BoneWeights[2];
	boneTransform += BoneMatrices[int(in_BoneIds[3])] * in_BoneWeights[3];

	vec4 position = boneTransform * in_Position;
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	gl_Position = MVP * position;
}
