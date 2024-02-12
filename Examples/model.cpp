#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

void AssimpToGLM(const aiMatrix4x4& a, glm::mat4& g)
{
	g[0][0] = a.a1; g[0][1] = a.b1; g[0][2] = a.c1; g[0][3] = a.d1;
	g[1][0] = a.a2; g[1][1] = a.b2; g[1][2] = a.c2; g[1][3] = a.d2;
	g[2][0] = a.a3; g[2][1] = a.b3; g[2][2] = a.c3; g[2][3] = a.d3;
	g[3][0] = a.a4; g[3][1] = a.b4; g[3][2] = a.c4; g[3][3] = a.d4;
}

Node* CreateAnimNode(aiNode* node)
{
	Node* newNode = new Node(node->mName.C_Str());
	AssimpToGLM(node->mTransformation, newNode->Transformation);
	return newNode;
}

Assimp::Importer m_AssimpImporter;

Model::Model()
{
}

Model::Model(const std::vector<glm::vec3>& Vertices, const std::vector<int>& triangles)
{
	unsigned int numVerticesInVertArray = Vertices.size();
	unsigned int numIndicesInIndexArray = triangles.size() * 3;
	sVertex_p4t4n4* pTempVertArray = new sVertex_p4t4n4[numIndicesInIndexArray * 2];
	GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];

	unsigned int vertArrayIndex = 0;
	for (unsigned int triIdx = 0; triIdx != triangles.size(); triIdx += 3)
	{
		glm::vec3 a = Vertices[triIdx];
		glm::vec3 b = Vertices[triIdx + 1];
		glm::vec3 c = Vertices[triIdx + 2];

		glm::vec3 ab = a - b;
		glm::vec3 ac = a - c;

		glm::vec3 normal = glm::cross(ab, ac);

		for (int vertIdx = 0; vertIdx != 3; vertIdx++)
		{
			unsigned int index = triIdx + vertIdx;

			glm::vec3 position = Vertices[index];
			pTempVertArray[vertArrayIndex].Pos.x = position.x;
			pTempVertArray[vertArrayIndex].Pos.y = position.y;
			pTempVertArray[vertArrayIndex].Pos.z = position.z;
			pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

			pTempVertArray[vertArrayIndex].TexUVx2.x = 0.0f;
			pTempVertArray[vertArrayIndex].TexUVx2.y = 0.0f;
			pTempVertArray[vertArrayIndex].TexUVx2.z = 0.0f;
			pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;

			pTempVertArray[vertArrayIndex].Normal.x = normal.x;
			pTempVertArray[vertArrayIndex].Normal.y = normal.y;
			pTempVertArray[vertArrayIndex].Normal.z = normal.z;
			pTempVertArray[vertArrayIndex].Normal.w = 0.0f;

			pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

			vertArrayIndex++;
		}
	}

	NumTriangles = triangles.size();

	glGenVertexArrays(1, &Vbo);
	glBindVertexArray(Vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &VertexBufferId);
	glGenBuffers(1, &IndexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

	unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4); ;
	glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);

	unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4);
	unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4, Pos);
	unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4, Normal);
	unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4, TexUVx2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);

	unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] pTempVertArray;
	delete[] pIndexArrayLocal;
}

struct BoneWeightInfo {
	BoneWeightInfo() {
		m_BoneId[0] = 0;
		m_BoneId[1] = 0;
		m_BoneId[2] = 0;
		m_BoneId[3] = 0;
		m_Weight[0] = 0.f;
		m_Weight[1] = 0.f;
		m_Weight[2] = 0.f;
		m_Weight[3] = 0.f;
	}
	float m_BoneId[4];
	float m_Weight[4];
};
#define PRINT_SPACES(x) {int s = x; while(s--> 0) printf(" ");}

Node* Model::GenerateBoneHierarchy(aiNode* assimpNode, const int depth)
{
	Node* node = CreateAnimNode(assimpNode);
	PRINT_SPACES(depth); printf("%s\n", assimpNode->mName.C_Str());
	aiMatrix4x4& transformation = assimpNode->mTransformation;
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scaling;
	transformation.Decompose(scaling, rotation, position);
	PRINT_SPACES(depth+1); printf("Position: {%.3f, %.3f, %.3f}\n", position.x, position.y, position.z);
	PRINT_SPACES(depth+1); printf("Scaling: {%.3f, %.3f, %.3f}\n", scaling.x, scaling.y, scaling.z);
	PRINT_SPACES(depth+1); printf("Quaternion: {%.3f, %.3f, %.3f, %.3f}\n", rotation.x, rotation.y, rotation.z, rotation.w);
	printf("\n");
	
	glm::mat4 glmMatrix;
	AssimpToGLM(transformation, glmMatrix);

	//glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, position.z));
	//glm::mat4 RotationMatrix = glm::mat4_cast(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
	//glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));

	//glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	NodeNameToIdMap.insert(std::pair<std::string, int>(assimpNode->mName.C_Str(), NodeHeirarchyTransformations.size()));
	NodeHeirarchyTransformations.emplace_back(glmMatrix);

	for (int i = 0; i < assimpNode->mNumChildren; ++i)
	{
		node->Children.emplace_back(GenerateBoneHierarchy(assimpNode->mChildren[i], depth + 1));
	}
	return node;
}

Model::Model(const char* filepath) {
	scene = (aiScene*)m_AssimpImporter.ReadFile(filepath, aiProcess_GenNormals);

	if (scene == 0 || !scene->HasMeshes())
		return;

	aiMesh* mesh = scene->mMeshes[0];

	if (scene->mNumAnimations > 0)
	{
		CharacterAnimation* characterAnimation = new CharacterAnimation();
		aiAnimation* animation = scene->mAnimations[0];

		characterAnimation->Name = animation->mName.C_Str();
		characterAnimation->Duration = animation->mDuration;
		characterAnimation->TicksPerSecond = animation->mTicksPerSecond;

		for (int i = 0; i < animation->mNumChannels; ++i)
		{
			aiNodeAnim* assimpNodeAnim = animation->mChannels[i];
			NodeAnim* nodeAnim = new NodeAnim(assimpNodeAnim->mNodeName.C_Str());
			for (int i = 0; i < assimpNodeAnim->mNumPositionKeys; ++i)
			{
				aiVectorKey& p = assimpNodeAnim->mPositionKeys[i];
				nodeAnim->m_PositionKeyFrames.emplace_back(PositionKeyFrame(glm::vec3(p.mValue.x, p.mValue.y, p.mValue.z), p.mTime));
			}
			for (int i = 0; i < assimpNodeAnim->mNumScalingKeys; ++i)
			{
				aiVectorKey& s = assimpNodeAnim->mScalingKeys[i];
				nodeAnim->m_ScaleKeyFrames.emplace_back(ScaleKeyFrame(glm::vec3(s.mValue.x, s.mValue.y, s.mValue.z), s.mTime));
			}
			for (int i = 0; i < assimpNodeAnim->mNumRotationKeys; ++i)
			{
				aiQuatKey& q = assimpNodeAnim->mRotationKeys[i];
				nodeAnim->m_RotationKeyFrames.emplace_back(RotationKeyFrame(glm::quat(q.mValue.w, q.mValue.x, q.mValue.y, q.mValue.z), q.mTime));
			}
			characterAnimation->Channels.emplace_back(nodeAnim);
		}
		CharacterAnimations.push_back(characterAnimation);
	}

	// std::map<int, BoneWeightInfo> boneMap;
	//				map			unorderd_map
	// Insertion	(Ln(N))			O(1) worst( O(n) ) hash collision
	// Lookup		(Ln(N))			O(1) worst( O(n) ) hash collision
	// We do not need a specific order
	// Data will not be changed

	// We can just build an array

	// Create our bone hierarchy
	// Maybe move this to a recursive function
	RootNode = GenerateBoneHierarchy(scene->mRootNode);
	GlobalInverseTransformation = glm::inverse(RootNode->Transformation);


	std::vector<BoneWeightInfo> boneWeights;

	if (mesh->HasBones())
	{
		boneWeights.resize(mesh->mNumVertices);
		unsigned int numBones = mesh->mNumBones;
		for (unsigned int boneIdx = 0; boneIdx < numBones; ++boneIdx)
		{
			aiBone* bone = mesh->mBones[boneIdx];

			std::string name(bone->mName.C_Str(), bone->mName.length); //	'\0'
			BoneNameToIdMap.insert(std::pair<std::string, int>(name, BoneInfoVec.size()));

			// Store the offset matrices
			BoneInfo info;
			AssimpToGLM(bone->mOffsetMatrix, info.BoneOffset);
			BoneInfoVec.emplace_back(info);
			printf("\n-----------\n");
			printf("Bone: %s\n", name.c_str());
			printf("Number of weights: %d\n", bone->mNumWeights);

			for (int weightIdx = 0; weightIdx < bone->mNumWeights; ++weightIdx)
			{
				aiVertexWeight& vertexWeight = bone->mWeights[weightIdx];
				// BoneId		:	boneIdx
				// Vertexid		:	vertexWeight.mVertexId
				// Weight		:	vertexWeight.mWeight

				BoneWeightInfo& boneInfo = boneWeights[vertexWeight.mVertexId];
				for (int infoIdx = 0; infoIdx < 4; ++infoIdx)
				{
					if (boneInfo.m_Weight[infoIdx] == 0.f)
					{
						boneInfo.m_BoneId[infoIdx] = boneIdx;
						boneInfo.m_Weight[infoIdx] = vertexWeight.mWeight;
						break;
					}
				}
			}
		}

		//for (int i = 0; i < mesh->mNumVertices; ++i)
		//{
		//	BoneWeightInfo& info = boneWeights[i];
		//	if (info.m_BoneId[0] < 0 || info.m_BoneId[0] > 4
		//		|| info.m_BoneId[1] < 0 || info.m_BoneId[1] > 4
		//		|| info.m_BoneId[2] < 0 || info.m_BoneId[2] > 4
		//		|| info.m_BoneId[3] < 0 || info.m_BoneId[3] > 4)
		//	printf("{%d:%d, %d:%d, %d:%d, %d:%d}\n",
		//		info.m_BoneId[0], info.m_Weight[0],
		//		info.m_BoneId[1], info.m_Weight[1],
		//		info.m_BoneId[2], info.m_Weight[2],
		//		info.m_BoneId[3], info.m_Weight[3]);
		//}
	}

	unsigned int numVerticesInVertArray = mesh->mNumVertices;
	unsigned int numIndicesInIndexArray = mesh->mNumFaces * 3;
	sVertex_p4t4n4b4w4* pTempVertArray = new sVertex_p4t4n4b4w4[numIndicesInIndexArray * 2];
	GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];
	int count = 0;
	int index = 0;

	//std::vector<int> boneCount;
	//boneCount.resize(5, 0);
	unsigned int vertArrayIndex = 0;
	for (unsigned int faceIdx = 0; faceIdx != mesh->mNumFaces; faceIdx++)
	{
		aiFace face = mesh->mFaces[faceIdx];

		for (int idx = 0; idx != 3; idx++)
		{
			unsigned int index = face.mIndices[idx];

			triangles.push_back(index);

			aiVector3D position = mesh->mVertices[index];
			pTempVertArray[vertArrayIndex].Pos.x = position.x;
			pTempVertArray[vertArrayIndex].Pos.y = position.y;
			pTempVertArray[vertArrayIndex].Pos.z = position.z;
			pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

			if (mesh->HasTextureCoords(0)) {
				aiVector3D textureCoord = mesh->mTextureCoords[0][index];
				pTempVertArray[vertArrayIndex].TexUVx2.x = textureCoord.x;
				pTempVertArray[vertArrayIndex].TexUVx2.y = textureCoord.y;
				pTempVertArray[vertArrayIndex].TexUVx2.z = textureCoord.z;
				pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
			}
			else
			{
				pTempVertArray[vertArrayIndex].TexUVx2.x = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.y = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.z = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
			}

			aiVector3D normal = mesh->mNormals[index];
			pTempVertArray[vertArrayIndex].Normal.x = normal.x;
			pTempVertArray[vertArrayIndex].Normal.y = normal.y;
			pTempVertArray[vertArrayIndex].Normal.z = normal.z;
			pTempVertArray[vertArrayIndex].Normal.w = 0.0f;

			if (mesh->HasBones())
			{
				BoneWeightInfo& boneInfo = boneWeights[index];

				pTempVertArray[vertArrayIndex].BoneIds.x = boneInfo.m_BoneId[0];
				pTempVertArray[vertArrayIndex].BoneIds.y = boneInfo.m_BoneId[1];
				pTempVertArray[vertArrayIndex].BoneIds.z = boneInfo.m_BoneId[2];
				pTempVertArray[vertArrayIndex].BoneIds.w = boneInfo.m_BoneId[3];

				pTempVertArray[vertArrayIndex].BoneWeights.x = boneInfo.m_Weight[0];
				pTempVertArray[vertArrayIndex].BoneWeights.y = boneInfo.m_Weight[1];
				pTempVertArray[vertArrayIndex].BoneWeights.z = boneInfo.m_Weight[2];
				pTempVertArray[vertArrayIndex].BoneWeights.w = boneInfo.m_Weight[3];

				float weight = boneInfo.m_Weight[0] + boneInfo.m_Weight[1] + boneInfo.m_Weight[2] + boneInfo.m_Weight[3];
				if (weight != 1.f)
				{
					int breakhere = 0;
				}
			}

			pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

			vertArrayIndex++;
		}
	}

	//if (mesh->HasBones())
	//{
	//	for (int i = 0; i < mesh->mNumVertices; ++i)
	//	{
	//		sVertex_p4t4n4b4w4& info = pTempVertArray[i];
	//		if (info.BoneIds.x < 0 || info.BoneIds.x > 4
	//			|| info.BoneIds.y < 0 || info.BoneIds.y > 4
	//			|| info.BoneIds.z < 0 || info.BoneIds.z > 4
	//			|| info.BoneIds.w < 0 || info.BoneIds.w > 4)
	//			printf("(%.2f, %.2f, %.2f, %.2f) (%.2f, %.2f, %.2f, %.2f) (%.2f, %.2f, %.2f, %.2f)\n",
	//				info.Pos.x, info.Pos.y, info.Pos.z, info.Pos.w,
	//				info.BoneIds.x, info.BoneIds.y, info.BoneIds.z, info.BoneIds.w,
	//				info.BoneWeights.x, info.BoneWeights.y, info.BoneWeights.z, info.BoneWeights.w);
	//	}
	//}
	printf("Loading %s mesh\n", filepath);
	//for (int i = 0; i < 50; ++i)
	//{
	//	printf("Bone Index %d has %d vertices\n", i, boneCount[i]);
	//}

	NumTriangles = mesh->mNumFaces;

	glGenVertexArrays(1, &Vbo);
	glBindVertexArray(Vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);	// bone ids
	glEnableVertexAttribArray(4);	// bone weights

	glGenBuffers(1, &VertexBufferId);
	glGenBuffers(1, &IndexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

	unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4b4w4);
	glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);

	unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4b4w4);
	unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4b4w4, Pos);
	unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4b4w4, Normal);
	unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4b4w4, TexUVx2);
	unsigned int byteOffsetToBoneIds = offsetof(sVertex_p4t4n4b4w4, BoneIds);
	unsigned int byteOffsetToBoneWeights = offsetof(sVertex_p4t4n4b4w4, BoneWeights);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneIds);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneWeights);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);

	unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] pTempVertArray;
	delete[] pIndexArrayLocal;
}

Model::Model(const char* filepath, bool withBones) {
	const aiScene* scene = m_AssimpImporter.ReadFile(filepath,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FixInfacingNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_ConvertToLeftHanded);

	aiMesh* mesh = scene->mMeshes[0];

	if (scene == 0 || !scene->HasMeshes())
		return;

	unsigned int numVerticesInVertArray = mesh->mNumVertices;
	unsigned int numIndicesInIndexArray = mesh->mNumFaces * 3;
	sVertex_p4t4n4b4w4* pTempVertArray = new sVertex_p4t4n4b4w4[numIndicesInIndexArray * 2];
	GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];
	int count = 0;
	int index = 0;

	for (unsigned int vertIdx = 0; vertIdx != mesh->mNumVertices; vertIdx++)
	{
		aiVector3D position = mesh->mVertices[vertIdx];
		Vertices.push_back(glm::vec3(position.x, position.y, position.z));
	}

	unsigned int vertArrayIndex = 0;
	for (unsigned int faceIdx = 0; faceIdx != mesh->mNumFaces; faceIdx++)
	{
		aiFace face = mesh->mFaces[faceIdx];

		for (int idx = 0; idx != 3; idx++)
		{
			unsigned int index = face.mIndices[idx];

			triangles.push_back(index);

			aiVector3D position = mesh->mVertices[index];
			pTempVertArray[vertArrayIndex].Pos.x = position.x;
			pTempVertArray[vertArrayIndex].Pos.y = position.y;
			pTempVertArray[vertArrayIndex].Pos.z = position.z;
			pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

			if (mesh->HasTextureCoords(0)) {
				aiVector3D textureCoord = mesh->mTextureCoords[0][index];
				pTempVertArray[vertArrayIndex].TexUVx2.x = textureCoord.x;
				pTempVertArray[vertArrayIndex].TexUVx2.y = textureCoord.y;
				pTempVertArray[vertArrayIndex].TexUVx2.z = textureCoord.z;
				pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
			}
			else
			{
				pTempVertArray[vertArrayIndex].TexUVx2.x = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.y = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.z = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
			}

			aiVector3D normal = mesh->mNormals[index];
			pTempVertArray[vertArrayIndex].Normal.x = normal.x;
			pTempVertArray[vertArrayIndex].Normal.y = normal.y;
			pTempVertArray[vertArrayIndex].Normal.z = normal.z;
			pTempVertArray[vertArrayIndex].Normal.w = 0.0f;

			pTempVertArray[vertArrayIndex].BoneIds.x = 0;
			pTempVertArray[vertArrayIndex].BoneIds.y = 1;
			pTempVertArray[vertArrayIndex].BoneIds.z = 2;
			pTempVertArray[vertArrayIndex].BoneIds.w = 3;

			glm::vec4 weights;
			weights.x = abs(0.f - position.y);
			weights.y = abs(70.f - position.y);
			weights.z = abs(140.f - position.y);
			weights.w = abs(200.f - position.y);

			weights = glm::normalize(weights);

			pTempVertArray[vertArrayIndex].BoneWeights.x = weights.x;
			pTempVertArray[vertArrayIndex].BoneWeights.y = weights.y;
			pTempVertArray[vertArrayIndex].BoneWeights.z = weights.z;
			pTempVertArray[vertArrayIndex].BoneWeights.w = weights.w;



			pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

			vertArrayIndex++;
		}
	}

	NumTriangles = mesh->mNumFaces;

	glGenVertexArrays(1, &Vbo);
	glBindVertexArray(Vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glGenBuffers(1, &VertexBufferId);
	glGenBuffers(1, &IndexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

	unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4b4w4); ;
	glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);

	unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4b4w4);
	unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4b4w4, Pos);
	unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4b4w4, TexUVx2);
	unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4b4w4, Normal);
	unsigned int byteOffsetToBoneIds = offsetof(sVertex_p4t4n4b4w4, BoneIds);
	unsigned int byteOffsetToBoneWeights = offsetof(sVertex_p4t4n4b4w4, BoneWeights);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneIds);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneWeights);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);

	unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] pTempVertArray;
	delete[] pIndexArrayLocal;
}

Model::~Model() {

}