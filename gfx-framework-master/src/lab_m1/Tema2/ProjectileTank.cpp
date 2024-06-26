#include "ProjectileTank.h"

ProjectileTank::ProjectileTank(glm::vec3 position, glm::vec3 direction)
{
	this->position = position;
	this->velocity = direction * PROJECTILE_SPEED;
	alive = true;
}

ProjectileTank::~ProjectileTank() 
{

}

void ProjectileTank::UpdatePosition(float deltaTime)
{
	velocity.y -= GRAVITY * deltaTime;
	position += velocity * deltaTime;
	if (position.y < 0)
	{
		alive = false;
	}
}