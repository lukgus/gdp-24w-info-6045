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

	void Update();

	void GetGameObjects(std::vector<GameObject*>& gameObjects) const;

	bool m_IsPaused = false;

	std::vector<Model*> m_Models;
	std::vector<ShaderProgram*> m_ShaderPrograms;
	std::vector<GameObject*> m_GameObjects;

	unsigned long long m_FrameNumber = 0;
};