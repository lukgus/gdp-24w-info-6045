#pragma once
#include <map>
#include <string>
#include <vector>

using gdpU32 = unsigned int;
using gdpS32 = int;

class Animation;
class Mesh;

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	void LoadFBXFromFile(const char* filename);

	std::vector<Animation*> m_Animations;
	std::vector<Mesh*> m_Meshes;
};