#pragma once
#include "components/simple_scene.h"
#define GRAVITY 2.0f
#define PROJECTILE_SPEED 6.0f

class ProjectileTank
{
public:
	glm::vec3 position, velocity;
	bool alive;
	ProjectileTank(glm::vec3 position, glm::vec3 direction);
	void UpdatePosition(float deltaTime);
	//Projectile();
	~ProjectileTank();

private:

};