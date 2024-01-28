#include "engine.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "gameobject.h"
#include "Shader.h"

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

		m_AnimationSystem.Update(gameObjects);
	}
}

void Engine::Render() const
{
	if (!m_Initialized)	{ return; }
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(45.0f),
		((GLfloat)m_Window->m_Width) / ((GLfloat)m_Window->m_Height),
		0.1f,
		10000.0f
	);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 1.0f, 20.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
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

	// Ask OpenGL to render our object
	glBindVertexArray(gameObject->m_Model->Vbo);
	glDrawElements(GL_TRIANGLES, gameObject->m_Model->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)0);

	for (GameObject* child : gameObject->m_Children)
	{
		RenderGameObject(child, ModelMatrix);
	}
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
