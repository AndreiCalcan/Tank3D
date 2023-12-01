#include "Tank.h"

Tank::Tank(glm::vec3 position) {
	this->position = position;
	base_rotation = 0;
	turret_rotation = 0;
	barrel_rotation = 0;
	forward = glm::vec3(0, 0, -1);
	hit_points = 3;
	attack_cd = 1.0;
	relative_dmg_pos = glm::vec3(0, 0, 0);
	alive = true;
	ai_enabled = true;
	state = rand() % 4;
	state_swap_chance = 0;
	chance_increase = 25;
	timer = 0;
}

void Tank::MoveForward(float deltaTimeSeconds)
{
	position = position + deltaTimeSeconds * forward;
}

void Tank::RotateTank(float deltaTimeSeconds)
{
	base_rotation += deltaTimeSeconds;
	forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), base_rotation, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1)));
}

void Tank::RotateTurret(float deltaTimeSeconds)
{
	turret_rotation += deltaTimeSeconds;
}

void Tank::RotateBarrel(float deltaTimeSeconds)
{
	if (barrel_rotation + deltaTimeSeconds > -0.20f && barrel_rotation + deltaTimeSeconds < 1.57f)
	{
		barrel_rotation += deltaTimeSeconds;
	}
		
}

ProjectileTank* Tank::GetNewProjectile()
{
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
	// Update shooting cooldown
	if (attack_cd < 1.0f)
	{
		attack_cd += deltaTimeSeconds;
	}

	// Exit if AI disabled
	if (!ai_enabled)
	{
		return;
	}

	// Check for state change
	timer += deltaTimeSeconds;
	if (timer > 1)
	{
		int change_roll = rand() % 100;
		if (change_roll < state_swap_chance)
		{
			int roll = rand() % 100;
			if (roll < 50)
			{
				state = FORWARD;
			}
			else if (roll < 70)
			{
				state = RIGHT;
			}
			else if (roll < 90)
			{
				state = LEFT;
			}
			else
			{
				state = BACK;
			}

			if (state == RIGHT || state == LEFT)
			{
				state_swap_chance = 30;
			}
			else
			{
				state_swap_chance = 0;
			}		
		}
		else
		{
			state_swap_chance += chance_increase * timer;
		}
		timer = 0;
	}

	switch (state)
	{
	case FORWARD:
		MoveForward(deltaTimeSeconds);
		break;
	case BACK:
		MoveForward(-deltaTimeSeconds);
		break;
	case RIGHT:
		RotateTank(-deltaTimeSeconds);
		break;
	case LEFT:
		RotateTank(deltaTimeSeconds);
		break;
	default:
		break;
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
	{
		hit_points--;
	}	
	else
	{
		ai_enabled = false;
	}
	relative_dmg_pos = damage_source - position;
	relative_dmg_pos = glm::vec3(glm::rotate(glm::mat4(1),-base_rotation, glm::vec3(0, 1, 0)) * glm::vec4(relative_dmg_pos, 1));
}