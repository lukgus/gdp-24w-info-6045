#pragma once

// TODO
// Move this to a better spot, in a common header file or a precomp.h
// Change these to 
// <extern/OpenGL/Glew/glew.h>
// <extern/math/glm/glm.hpp>
// ...
#include <GLEW/glew.h>
#include <glm/glm.hpp>
// end TODO

#include <assimp/scene.h>
#include <map>
#include <string>
#include <vector>

#include "animation.h"

class aiNode;

typedef struct sFloat4 {
	float x, y, z, w;
} sFloat4;

typedef struct sUint4 {
	unsigned int x, y, z, w;
} sUint4;

typedef struct sInt4 {
	int x, y, z, w;
} sInt4;

typedef struct sVertex_p4t4n4 {
	sFloat4 Pos;
	sFloat4 TexUVx2;
	sFloat4 Normal;
} sVertex_p4t4n4;

typedef struct sVertex_p4t4n4b4 {
	sFloat4 Pos;
	sFloat4 TexUVx2;
	sFloat4 Normal;
	sFloat4 BoneIds;
} sVertex_p4t4n4b4;


typedef struct sVertex_p4t4n4b4w4 {
	sFloat4 Pos;
	sFloat4 TexUVx2;
	sFloat4 Normal;
	sFloat4 BoneWeights;
	sFloat4 BoneIds;
} sVertex_p4t4n4b4w4;

struct BoneInfo
{
	glm::mat4 BoneOffset;				// Offset from the parent bone/node
	glm::mat4 FinalTransformation;		// Calculated transformation used for rendering
	glm::mat4 GlobalTransformation;		// used for the bone hierarchy transformation calculations when animating
};

// Connection Node for hierarchy
struct Node
{
	Node(const std::string& name) : Name(name) { }
	std::string Name;
	glm::mat4 Transformation;
	std::vector<Node*> Children;
};

// Animation Node
struct NodeAnim
{
	NodeAnim(const std::string& name) : Name(name) { }
	std::string Name;
	std::vector<PositionKeyFrame> m_PositionKeyFrames;
	std::vector<ScaleKeyFrame> m_ScaleKeyFrames;
	std::vector<RotationKeyFrame> m_RotationKeyFrames;
};

struct CharacterAnimation
{
	std::string Name;
	double TicksPerSecond;
	double Duration;
	Node* RootNode;
	std::vector<NodeAnim*> Channels;
};

void AssimpToGLM(const aiMatrix4x4& a, glm::mat4& g);

class Model {
public:
	Model();
	Model(const char* filepath);
	Model(const char* filepath, bool withBones);
	Model(const std::vector<glm::vec3>& Vertices, const std::vector<int>& triangles);
	~Model();

	Node* GenerateBoneHierarchy(aiNode* node, const int depth = 0);

	GLuint Vbo;
	GLuint VertexBufferId;
	GLuint IndexBufferId;

	// using std::string screams performance issues. This will just be used for learning
	// how to setup an animation and see how everything is connected. We can replace this
	// in the future once we have everything working.
	std::vector<glm::mat4> NodeHeirarchyTransformations;
	std::map<std::string, int> NodeNameToIdMap;

	std::vector<BoneInfo> BoneInfoVec;
	std::map<std::string, int> BoneNameToIdMap;
	Node* RootNode;

	std::vector<CharacterAnimation*> CharacterAnimations;

	glm::mat4 GlobalInverseTransformation;

	std::vector<glm::vec3> Vertices;
	std::vector<int> triangles;	// 1,2,3
	aiScene* scene;
	unsigned int NumTriangles;
};