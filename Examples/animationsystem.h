#pragma once

#include "gameobject.h"

class aiNode;
class AnimationSystem
{
public:
	void Update(const std::vector<GameObject*>& gameObjects) const;
	void UpdateCharacter(const std::vector<GameObject*>& gameObjects) const;
private:
	void CalculateMatrices(Model* model, CharacterAnimation* animation, Node* node, const glm::mat4& parentTransformationMatrix, double keyFrameTime) const;
};