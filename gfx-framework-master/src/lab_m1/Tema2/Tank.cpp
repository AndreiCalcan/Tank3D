#include "Tank.h"

Tank::Tank(glm::vec3 position) {
	this->position = position;
	base_rotation = 0;
	turret_rotation = 0;
	barrel_rotation = 0;
	forward = glm::vec3(0, 0, -1);
	hit_points = 3;
	attack_cd = 1.0;
	damage_position = glm::vec3(0, 0, 0);
}

void Tank::MoveForward(float deltaTimeSeconds)
{
	position = position + deltaTimeSeconds * forward;
}

void Tank::RotateTank(float deltaTimeSeconds)
{
	base_rotation += deltaTimeSeconds;
	forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), base_rotation, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1)));
	//cannon_dir = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), deltaTimeSeconds, glm::vec3(0, 1, 0)) * glm::vec4(cannon_dir, 1)));
}

void Tank::RotateTurret(float deltaTimeSeconds)
{
	turret_rotation += deltaTimeSeconds;
	//cannon_dir = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), deltaTimeSeconds, glm::vec3(0, 1, 0)) * glm::vec4(cannon_dir, 1)));
}

void Tank::RotateBarrel(float deltaTimeSeconds)
{
	if (barrel_rotation + deltaTimeSeconds > -0.20f && barrel_rotation + deltaTimeSeconds < 1.57f)
	{
		barrel_rotation += deltaTimeSeconds;
		//cannon_dir = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), deltaTimeSeconds, forward) * glm::vec4(cannon_dir, 1)));
	}
		
}

ProjectileTank* Tank::GetNewProjectile()
{
	/*cannon_dir = glm::normalize(BALL_OFFSET - BARREL_OFFSET);

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::rotate(modelMatrix, base_rotation, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, turret_rotation, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, barrel_rotation, glm::vec3(1, 0, 0));

	cannon_dir = glm::normalize(glm::vec3(modelMatrix * glm::vec4(cannon_dir, 1)));
	return cannon_dir;*/
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, base_rotation, glm::vec3(0, 1, 0));
	modelMatrix = glm::translate(modelMatrix, MESH_SCALE * glm::vec3(0, 2.0478f, 1.2675f));
	modelMatrix = glm::rotate(modelMatrix, turret_rotation, glm::vec3(0, 1, 0));
	modelMatrix = glm::translate(modelMatrix, MESH_SCALE * glm::vec3(0, 2.2667f, -0.2f) - MESH_SCALE * glm::vec3(0, 2.0478f, 1.2675f));
	modelMatrix = glm::rotate(modelMatrix, barrel_rotation, glm::vec3(1, 0, 0));

	glm::vec3 ball_pos = glm::vec3(modelMatrix * glm::vec4(BALL_OFFSET - BARREL_OFFSET, 1));
	glm::vec3 barrel_pos = glm::vec3(modelMatrix * glm::vec4(glm::vec3(0), 1));
	glm::vec3 dir = glm::normalize(ball_pos - barrel_pos);
	return new ProjectileTank(ball_pos, dir);
}

void Tank::Update(float deltaTimeSeconds)
{
	if (attack_cd < 1.0f)
	{
		attack_cd += deltaTimeSeconds;
	}
}

bool Tank::CanShoot()
{
	return attack_cd >= 1.0f;
}

ProjectileTank* Tank::Shoot()
{
	attack_cd = 0;
	return GetNewProjectile();
}

void Tank::TakeDamage(glm::vec3 damage_source)
{
	if (hit_points > 0)
		hit_points--;
	damage_position = damage_source;
}