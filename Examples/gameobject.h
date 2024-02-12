#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "component.h"
#include "shader.h"
#include "model.h"
#include "animation.h"

struct GameObject
{
	GameObject();
	virtual ~GameObject();

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

	std::vector<GameObject*> m_Children;
	std::vector<Component*> m_Components;
};

struct Character : public GameObject
{
	~Character() override { }
	std::vector<glm::mat4> m_BoneTransformations;
	Animation* m_Animation;
};