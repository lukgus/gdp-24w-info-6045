#pragma once

#include "gameobject.h"

class AnimationSystem
{
public:
	void Update(const std::vector<GameObject*>& gameObjects) const;
};