#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "component.h"
#include "shader.h"
#include "model.h"
#include "animation.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	unsigned long long m_Id;
	const char* m_Name;
	bool m_Visible = true;
	bool m_Enabled = true;

	ShaderProgram* m_ShaderProgram;
	Model* m_Model;

	glm::vec3 m_Position = glm::vec3(0.f);
	glm::vec3 m_Scale = glm::vec3(1.f);
	glm::quat m_Rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
	bool m_DirtyTransform;
	// glm::mat4 m_WorldModelMatrix;
	// glm::mat4 m_LocalModelMatrix;

	Animation* m_Animation;

	std::vector<GameObject*> m_Children;
	std::vector<Component*> m_Components;
};