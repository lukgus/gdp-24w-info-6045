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

#include <map>
#include <string>
#include <vector>

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


class Model {
public:
	Model();
	Model(const char* filepath);
	Model(const char* filepath, bool withBones);
	Model(const std::vector<glm::vec3>& Vertices, const std::vector<int>& triangles);
	~Model();

	void GenerateBoneHierarchy(aiNode* node, const int depth = 0);

	GLuint Vbo;
	GLuint VertexBufferId;
	GLuint IndexBufferId;

	// using std::string screams performance issues. This will just be used for learning
	// how to setup an animation and see how everything is connected. We can replace this
	// in the future once we have everything working.
	std::vector<glm::mat4> LocalBoneTransformations;
	std::map<std::string, int> BoneNameToIdMap;

	std::vector<glm::vec3> Vertices;
	std::vector<int> triangles;	// 1,2,3

	unsigned int NumTriangles;
};