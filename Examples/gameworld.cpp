#include "gameworld.h"
#include <glm/gtx/easing.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	//Model* model1 = new Model("assets/models/cone.obj");
	//Model* model2 = new Model("assets/models/cone-pivot-bottom.obj");
	//Model* model3 = new Model("assets/models/cone-pivot-bottom-subdivided.obj");
	//Model* model4 = new Model("assets/models/cone-subdivided-oriented-boned.fbx");
	Model* model5 = new Model("assets/models/Adventurer Aland@Idle.FBX");
	//Model* model4 = new Model("assets/models/Adventurer Aland@Idle.FBX");
	//m_Models.push_back(model1);
	//m_Models.push_back(model2);
	//m_Models.push_back(model3);
	//m_Models.push_back(model4);
	m_Models.push_back(model5);

	// RunExample1();		// KeyFrame Animation
	// RunExample2();		// Hierarchy Animation
	 //RunExample3();		// Bone Animation with Shader
	 RunExample4();		// Bone Animation with Shader
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

void UpdateAnimationTimes(GameObject* gameObject, float time)
{
	if (gameObject->m_Animation != nullptr)
	{
		gameObject->m_Animation->m_Time = time;
	}

	for (GameObject* childObject : gameObject->m_Children)
	{
		UpdateAnimationTimes(childObject, time);
	}
}

void GameWorld::Update()
{
	if (m_IsPaused) return;

	m_FrameNumber++;
	if (m_FrameNumber > 1000) m_FrameNumber = 0;
	float val = (float)m_FrameNumber / 250.f;

	for (GameObject* gameObject : m_GameObjects)
	{
		UpdateAnimationTimes(gameObject, val);
	}
	if (exampleNumber == 3)
	{
		glm::mat4 boneMatrices[5];
		for (int i = 0; i < 5; ++i)
		{
			boneMatrices[i] = glm::mat4(1.f);
		}

		// Calculate bone matrices for Example3
		GameObject* rootAnimationGameObject = m_GameObjects[0];
		glm::mat4 parentModelMatrix = glm::mat4(1.f);	// start with identity matrix
		int boneMatrixIndex = 0;
		while (rootAnimationGameObject != nullptr)
		{
			// You can calculate this when a game object is set as "Dirty" 
			glm::mat4 TranslationMatrix = glm::translate(parentModelMatrix, rootAnimationGameObject->m_Position);
			//glm::mat4 RotationMatrix = glm::mat4_cast(rootAnimationGameObject->m_Rotation);
			//glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), rootAnimationGameObject->m_Scale);

			// Matrix multiplications happen right to left
			// For this calculation it will calculate the scale, then rotation then translation
			boneMatrices[boneMatrixIndex++] = TranslationMatrix;// *RotationMatrix* ScaleMatrix;
			parentModelMatrix = TranslationMatrix;
			rootAnimationGameObject = rootAnimationGameObject->m_Children.size() > 0 ? rootAnimationGameObject->m_Children[0] : nullptr;
		}

		for (int i = 0; i < 5; ++i)
		{
			ShaderProgram* shader = m_ShaderPrograms[0];
			glUseProgram(shader->id);
			glUniformMatrix4fv(shader->BoneMatricesUL[i], 1, GL_FALSE, glm::value_ptr(boneMatrices[i]));
		}

		// We will move this next class. just to see the example working today:
		// Send the bone matrices data to the shader here.
	}
	if (exampleNumber == 4)
	{

		glm::mat4 boneMatrices[50];
		for (int i = 0; i < 5; ++i)
		{
			boneMatrices[i] = glm::mat4(1.f);
		}


		for (int i = 0; i < 50; ++i)
		{
			ShaderProgram* shader = m_ShaderPrograms[0];
			glUseProgram(shader->id);
			glUniformMatrix4fv(shader->BoneMatricesUL[i], 1, GL_FALSE, glm::value_ptr(boneMatrices[i]));
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


void GameWorld::RunExample1()
{
	Animation* animation = new Animation();
	animation->m_Time = 0.0;
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, -5, 0), 0.0) /* No Easing Type Specified */);
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, -5, 0), 1.0, EasingType::sineEaseIn));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, 5, 0), 2.0, EasingType::Linear));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 5, 0), 3.0, EasingType::sineEaseInOut));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, -5, 0), 4.0, EasingType::sineEaseOut));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 0.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 1.f, 0.f, 0.f), 1.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 1.f, 0.f), 2.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 0.f, 1.f), 3.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 4.0));

	GameObject* goA = new GameObject();
	goA->m_Model = m_Models[0];
	goA->m_ShaderProgram = m_ShaderPrograms[0];
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

void GameWorld::RunExample2()
{
	const float coneHeight = 2.f;
	Model* coneModel = m_Models[3];
	Animation* animation = new Animation();
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.0f, coneHeight, 0.f), 0.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 0.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 1.f, 0.f, 0.f), 1.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 1.f, 0.f), 2.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 0.f, 1.f), 3.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 4.0));
	animation->m_Time = 0.0;

	GameObject* goA = new GameObject();
	GameObject* goB = new GameObject();
	GameObject* goC = new GameObject();
	GameObject* goD = new GameObject();

	goA->m_Model = coneModel;
	goA->m_ShaderProgram = m_ShaderPrograms[0];
	goA->m_Animation = animation;
	m_GameObjects.push_back(goA);

	goB->m_Model = coneModel;
	goB->m_Position.y = coneHeight;
	goB->m_Animation = animation;
	goB->m_ShaderProgram = m_ShaderPrograms[0];
	//m_GameObjects.push_back(goB);

	goC->m_Model = coneModel;
	goC->m_Position.y = coneHeight;
	goC->m_Animation = animation;
	goC->m_ShaderProgram = m_ShaderPrograms[0];
	//m_GameObjects.push_back(goC);

	goD->m_Model = coneModel;
	goD->m_ShaderProgram = m_ShaderPrograms[0];
	goD->m_Position.y = coneHeight;
	goD->m_Animation = animation;
	//m_GameObjects.push_back(goD);

	goA->m_Children.push_back(goB);
	goB->m_Children.push_back(goC);
	goC->m_Children.push_back(goD);
}

void GameWorld::RunExample3()
{
	exampleNumber = 3;
	const float coneHeight = 2.f;
	Model* coneModel = m_Models[0];
	Animation* animation = new Animation();
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.0f, 0.f, 0.f), 0.0));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(1.0f, 0.f, 0.f), 1.0));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-1.0f, 0.f, 0.f), 3.0));
	animation->m_PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.0f, 0.f, 0.f), 4.0));
	animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 0.0));
	//animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 1.f, 0.f, 0.f), 1.0));
	//animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 1.f, 0.f), 2.0));
	//animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.0f, 0.f, 0.f, 1.f), 3.0));
	//animation->m_RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 4.0));
	animation->m_Time = 0.0;

	// This builds our bone hierarchy.
	GameObject* goA = new GameObject();
	GameObject* goB = new GameObject();
	GameObject* goC = new GameObject();
	GameObject* goD = new GameObject();
	GameObject* goE = new GameObject();

	goA->m_Visible = false;

	goA->m_Model = coneModel;
	goA->m_ShaderProgram = m_ShaderPrograms[0];
	goA->m_Animation = animation;
	m_GameObjects.push_back(goA);

	goB->m_Model = coneModel;
	goB->m_Position.y = coneHeight;	// translation offset
	goB->m_Animation = animation;
	goB->m_ShaderProgram = m_ShaderPrograms[0];
	//m_GameObjects.push_back(goB);

	goC->m_Model = coneModel;
	goC->m_Position.y = coneHeight;	// translation offset
	goC->m_Animation = animation;
	goC->m_ShaderProgram = m_ShaderPrograms[0];
	//m_GameObjects.push_back(goC);

	goD->m_Model = coneModel;
	goD->m_ShaderProgram = m_ShaderPrograms[0];
	goD->m_Position.y = coneHeight;	// translation offset
	goD->m_Animation = animation;
	//m_GameObjects.push_back(goD);

	goE->m_Model = coneModel;
	goE->m_ShaderProgram = m_ShaderPrograms[0];
	goE->m_Position.y = coneHeight;	// translation offset
	goE->m_Animation = animation;
	//m_GameObjects.push_back(goE);

	goA->m_Children.push_back(goB);
	goB->m_Children.push_back(goC);
	goC->m_Children.push_back(goD);
	goD->m_Children.push_back(goE);

	Model* bigConeModel = m_Models[0];
	GameObject* goBigCone = new GameObject();

	goBigCone->m_Model = bigConeModel;
	goBigCone->m_ShaderProgram = m_ShaderPrograms[0];
	m_GameObjects.push_back(goBigCone);
}

void GameWorld::RunExample4()
{
	exampleNumber = 4;

	GameObject* aland = new GameObject();
	aland->m_Model = m_Models[0];
	aland->m_ShaderProgram = m_ShaderPrograms[0];
	m_GameObjects.push_back(aland);

}