#include "engine.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "gameobject.h"
#include <Shader.h>

Engine::Engine(const char* windowTitle, int width, int height)
{
	m_Window = new Window(windowTitle, width, height);
}

Engine::~Engine()
{
}

void Engine::Initialize()
{
	if (m_Initialized)
	{
		return;
	}

	m_Initialized = true;
}

void Engine::Destroy()
{
	if (!m_Initialized)
	{
		return;
	}

	m_Initialized = false;
}

void Engine::LoadWorld(const char* world)
{
	if (m_WorldLoaded)
	{
		printf("Error: World is already loaded. Unload the world first before loading a new world.\n");
		return;
	}

	m_World = new GameWorld(world);
	m_World->StartUp();

	m_WorldLoaded = true;
}

void Engine::UnloadWorld()
{
	if (!m_WorldLoaded)
	{
		return;
	}

	m_World->Shutdown();
	m_WorldLoaded = false;
}

void Engine::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Engine::Update()
{
	if (!m_Initialized)
	{
		return;
	}

	if (m_WorldLoaded && m_World != nullptr)
	{
		m_World->Update();

		std::vector<GameObject*> gameObjects;
		m_World->GetGameObjects(gameObjects);

		for (int i = 0; i < gameObjects.size(); ++i)
		{
			gameObjects[i]->m_Rotation = glm::rotate(gameObjects[i]->m_Rotation, 0.002f, glm::vec3(0.f, 1.f, 0.f));
		}

		m_AnimationSystem.Update(gameObjects);
		m_AnimationSystem.UpdateCharacter(gameObjects);
	}
}

void Engine::Render() const
{
	if (!m_Initialized)	{ return; }
	glClearColor(0.4f, 0.4f, 0.4f, 1.f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(45.0f),
		((GLfloat)m_Window->m_Width) / ((GLfloat)m_Window->m_Height),
		0.1f,
		10000.0f
	);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 100.0f, 300.0f),
		glm::vec3(0.0f, 100.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	ShaderProgram* shader = m_World->GetShaderProgram(0);
	glUseProgram(shader->id);
	glUniformMatrix4fv(shader->ProjectionMatrixUL, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->ViewMatrixUL, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	std::vector<GameObject*> m_GameObjects;
	m_World->GetGameObjects(m_GameObjects);

	glm::mat4 worldOriginPoint = glm::mat4(1.f);	// Set as Identity matrix (origin point)
	for (GameObject* gameObject : m_GameObjects)
	{
		RenderGameObject(gameObject, worldOriginPoint);
	}
}
void Engine::RenderGameObject(GameObject* gameObject, const glm::mat4& parentModelMatrix) const
{
	if (gameObject->m_Visible == false)
	{
		return;
	}

	ShaderProgram* shader = gameObject->m_ShaderProgram;

	// You can calculate this when a game object is set as "Dirty" 
	glm::mat4 TranslationMatrix = glm::translate(parentModelMatrix, gameObject->m_Position);
	glm::mat4 RotationMatrix = glm::mat4_cast(gameObject->m_Rotation);
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), gameObject->m_Scale);

	// Matrix multiplications happen right to left
	// For this calculation it will calculate the scale, then rotation then translation
	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	// Send ModelMatrix to our shader
	glUniformMatrix4fv(shader->ModelMatrixUL, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform1i(shader->UseBonesUL, true);
	// Ask OpenGL to render our object
	for (int i = 0; i < gameObject->m_Model->BoneInfoVec.size(); ++i)
	{
		glUniformMatrix4fv(shader->BoneMatricesUL[i], 1, GL_FALSE, glm::value_ptr(gameObject->m_Model->BoneInfoVec[i].FinalTransformation));
	}

	glBindVertexArray(gameObject->m_Model->Vbo);
	glDrawElements(GL_TRIANGLES, gameObject->m_Model->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)0);

	// We can render all of our bone to the screen here:
	glUniform1i(shader->UseBonesUL, false);
	for (int i = 0; i < gameObject->m_Model->BoneInfoVec.size(); ++i)
	{
		glUniformMatrix4fv(shader->ModelMatrixUL, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));// gameObject->m_Model->BoneInfoVec[i].GlobalTransformation));
		RenderBoneDebug();
	}

	for (GameObject* child : gameObject->m_Children)
	{
		RenderGameObject(child, ModelMatrix);
	}
}

void Engine::RenderBoneDebug() const
{
	const Model* boneDebugModel = m_World->GetBoneDebugModel();
	glBindVertexArray(boneDebugModel->Vbo);
	glDrawElements(GL_TRIANGLES, boneDebugModel->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)0);

}

void Engine::KeyPress(unsigned char key)
{
}

void Engine::KeyRelease(unsigned char key)
{
}

void Engine::MouseMotion(double x, double y)
{
}
