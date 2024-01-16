#pragma once

#include "component.h"
#include "gameobject.h"

class GameScript : public Component
{
public:
	virtual ~GameScript() { }
protected:
	GameScript() { }

	virtual void Update(GameObject* gameObject) = 0;
};

class PlayerController : public GameScript
{
public:
	PlayerController() : GameScript() { }
	virtual ~PlayerController() { }

	void Update(GameObject* gameObject) override
	{

	}
};