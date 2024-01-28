#pragma once

#include <vector>

#include "gameobject.h"
#include "shader.h"
#include "model.h"

class GameWorld
{
public:
	GameWorld(const char* world);
	~GameWorld();

	void StartUp();
	void Shutdown();

	void Pause();
	void Resume();
	inline bool IsPaused() const { return m_IsPaused; }
	inline ShaderProgram* GetShaderProgram(int id) const {
		return m_ShaderPrograms[id];
	}

	void Update();

	void GetGameObjects(std::vector<GameObject*>& gameObjects) const;

private:
	int exampleNumber = 0;
	void RunExample1();		// Keyframe Animation
	void RunExample2();		// Hierarchy Animation
	void RunExample3();		// Bone Animation

	bool m_IsPaused = false;

	std::vector<Model*> m_Models;
	std::vector<ShaderProgram*> m_ShaderPrograms;
	std::vector<GameObject*> m_GameObjects;

	unsigned long long m_FrameNumber = 0;
};