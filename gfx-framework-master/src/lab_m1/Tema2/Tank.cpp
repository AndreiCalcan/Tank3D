#include "Tank.h"
#include <iostream>

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
	position = position + 2 * deltaTimeSeconds * forward;
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

void Tank::Update(float deltaTimeSeconds, Tank *player_tank)
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

	if (glm::distance(position, player_tank->position) < 10.0f)
	{
		state = TARGETING;
		return;
	}

	// Check for state change
	timer += deltaTimeSeconds;
	if (timer > 1 && state != TARGETING)
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
	case TARGETING:
		state = rand() % 4;
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
		alive = false;
	}
	relative_dmg_pos = damage_source - position;
	relative_dmg_pos = glm::vec3(glm::rotate(glm::mat4(1),-base_rotation, glm::vec3(0, 1, 0)) * glm::vec4(relative_dmg_pos, 1));
	relative_dmg_pos = glm::normalize(relative_dmg_pos) * MESH_SCALE * 3.8f;
}

ProjectileTank* Tank::Target(float deltaTimeSeconds, Tank* player_tank)
{
	if (ai_enabled && TargetHorizontal(deltaTimeSeconds, player_tank) && CanShoot() && TargetVertical(deltaTimeSeconds, player_tank))
	{
		return Shoot();
	}
	return NULL;
}

bool Tank::TargetHorizontal(float deltaTimeSeconds, Tank *player_tank)
{
	glm::vec3 correct_dir = glm::normalize(player_tank->position - position);
	glm::vec3 curr_dir = glm::vec3(glm::rotate(glm::mat4(1), turret_rotation, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1));
	curr_dir = glm::normalize(curr_dir);
	float cos_alfa = glm::dot(correct_dir, curr_dir);
	if (cos_alfa > 0.9999f)
	{
		
		return true;
	}
	//std::cout << cos_alfa << endl;
	glm::vec3 next_dir1 = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), deltaTimeSeconds, glm::vec3(0, 1, 0)) * glm::vec4(curr_dir, 1)));
	glm::vec3 next_dir2 = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), -deltaTimeSeconds, glm::vec3(0, 1, 0)) * glm::vec4(curr_dir, 1)));
	float cos_alfa1 = glm::dot(correct_dir, next_dir1);
	float cos_alfa2 = glm::dot(correct_dir, next_dir2);
	if (cos_alfa1 > cos_alfa2)
	{
		RotateTurret(deltaTimeSeconds);
		return false;
	}
	RotateTurret(-deltaTimeSeconds);
	return false;
}

bool Tank::TargetVertical(float deltaTimeSeconds, Tank* player_tank)
{
	float dist = glm::distance(position, player_tank->position) - 5.35f * MESH_SCALE;
	float v_sqr = PROJECTILE_SPEED * PROJECTILE_SPEED;
	float correct_angle = asin((GRAVITY * dist) / v_sqr) * 0.5f;
	//cout << correct_angle << " " << barrel_rotation << endl;
	if (abs(correct_angle - barrel_rotation) < RADIANS(0.1f))
	{
		//cout << abs(correct_angle - barrel_rotation) << " < " << RADIANS(1.0f) << endl;
		return true;
	}

	if (barrel_rotation < correct_angle)
	{
		RotateBarrel(deltaTimeSeconds);
		return false;
	}

	RotateBarrel(-deltaTimeSeconds);
	return false;
}