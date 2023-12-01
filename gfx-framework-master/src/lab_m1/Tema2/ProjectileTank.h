#pragma once
#include "components/simple_scene.h"
#define GRAVITY 5.0f
#define PROJECTILE_SPEED 8.0f

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