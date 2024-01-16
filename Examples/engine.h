#pragma once

#include "gameworld.h";
#include "window.h"
#include "AnimationSystem.h"

class Engine
{
public:
	Engine(const char* windowTitle, int width, int height);
	~Engine();

	void Initialize();
	void Destroy();
	inline bool IsInitialied() const { return m_Initialized; }

	void LoadWorld(const char* world);
	void UnloadWorld();
	inline bool IsWorldLoaded() const { return m_WorldLoaded; }

	void Update();
	void Render() const;
	void RenderGameObject(GameObject* gameObject, const glm::mat4& parentModelMatrix) const;

	void KeyPress(unsigned char key);
	void KeyRelease(unsigned char key);
	void MouseMotion(double x, double y);

private:
	bool m_Initialized = false;
	bool m_WorldLoaded = false;
	GameWorld* m_World;
	Window* m_Window;
	AnimationSystem m_AnimationSystem;
};