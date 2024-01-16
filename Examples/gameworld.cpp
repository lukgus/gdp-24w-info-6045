#include "gameworld.h"
#include <glm/gtx/easing.hpp>
#include <glm/gtx/quaternion.hpp>

GameWorld::GameWorld(const char* world)
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::StartUp()
{
	const char* vertexFilepath = "assets/shaders/simpleshader.vertex.glsl";
	const char* fragmentFilepath = "assets/shaders/simpleshader.fragment.glsl";
	ShaderProgram* simpleShader = new ShaderProgram(vertexFilepath, fragmentFilepath);
	m_ShaderPrograms.push_back(simpleShader);

	Model* model = new Model("assets/models/cone.obj");
	m_Models.push_back(model);

	Animation* animation = new Animation();
	animation->m_Time = 0.0;
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, -5, 0), 0.0));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3( 5, -5, 0), 1.0, EasingType::sineEaseIn));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3( 5,  5, 0), 2.0, EasingType::Linear));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5,  5, 0), 3.0, EasingType::Linear));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, -5, 0), 4.0, EasingType::sineEaseOut));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f ,0.f, 0.f), 0.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 1.f, 0.f, 0.f), 1.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 1.f, 0.f), 2.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 0.f, 1.f), 3.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 4.0));

	GameObject* goA = new GameObject();
	goA->m_Model = model;
	goA->m_ShaderProgram = simpleShader;
	goA->m_Animation = animation;
	m_GameObjects.push_back(goA);

	//GameObject* goB = new GameObject();
	//goB->m_Model = model;
	//goB->m_ShaderProgram = simpleShader;
	//goB->m_Animation = new Animation(glm::vec3(-5.f, 0.f, 0.f), glm::vec3(5.f, 0.f, 0.f), EasingType::sineEaseOut);
	//m_GameObjects.push_back(goB);

	//GameObject* goC = new GameObject();
	//goC->m_Position.y = -2.f;
	//goC->m_Model = model;
	//goC->m_ShaderProgram = simpleShader;
	//goC->m_Animation = new Animation(glm::vec3(-5.f, -2.f, 0.f), glm::vec3(5.f, -2.f, 0.f), EasingType::sineEaseInOut);
	//m_GameObjects.push_back(goC);

	//GameObject* goD = new GameObject();
	//goD->m_Position.y = -4.f;
	//goD->m_Model = model;
	//goD->m_ShaderProgram = simpleShader;
	//m_GameObjects.push_back(goD);

	//GameObject* child = new GameObject();
	//child->m_Position.x = 4.f;
	//child->m_Scale.x = 1.5f;
	//child->m_Model = model;
	//child->m_ShaderProgram = simpleShader;
	//root->m_Children.push_back(child);

	//GameObject* grandchild = new GameObject();
	//grandchild->m_Position.x = 4.f;
	//grandchild->m_Model = model;
	//grandchild->m_ShaderProgram = simpleShader;
	//child->m_Children.push_back(grandchild);
	//m_GameObjects.push_back(grandchild);
}

void GameWorld::Shutdown()
{
}

void GameWorld::Pause()
{
	m_IsPaused = true;
}

void GameWorld::Resume()
{
	m_IsPaused = false;
}

void GameWorld::Update()
{
	if (m_IsPaused) return;

	m_FrameNumber++;
	if (m_FrameNumber > 1000) m_FrameNumber = 0;
	float val = (float)m_FrameNumber / 250.f;

	for (GameObject* gameObject : m_GameObjects)
	{
		if (gameObject->m_Animation != nullptr)
		{
			gameObject->m_Animation->m_Time = val;
		}
	}

	// Looking for a value 0 to 1,(from start to finish)
	// distance = 5 - -5 = 10
	// startpoint = -5
	//m_GameObjects[0]->m_Position.x = glm::sineEaseIn(val) * 10 -5;		// Returns [0, 1]
	//m_GameObjects[1]->m_Position.x = glm::sineEaseInOut(val) * 10 -5;	// Returns [0, 1]
	//m_GameObjects[2]->m_Position.x = glm::sineEaseOut(val) * 10 -5;		// Returns [0, 1]
	//m_GameObjects[3]->m_Position.x = glm::bounceEaseIn(val) * 10 -5;		// Returns [0, 1]
}

void GameWorld::GetGameObjects(std::vector<GameObject*>& gameObjects) const
{
	gameObjects = m_GameObjects;
}