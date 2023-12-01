#pragma once
#include "components/simple_scene.h"
#include "ProjectileTank.h"
using namespace std;

#define MESH_SCALE 0.15f
#define BALL_OFFSET glm::vec3(0, 2.2073f, -5.35f) * MESH_SCALE
#define BARREL_OFFSET glm::vec3(0, 2.2667f, -0.2f) * MESH_SCALE
#define TURRET_OFFSET MESH_SCALE * glm::vec3(0, 2.0478f, 1.2675f)
#define TANK_RADIUS MESH_SCALE * 4.4f
#define PROJECTILE_RADIUS MESH_SCALE * 0.2f
#define FORWARD 0
#define BACK 1
#define RIGHT 2
#define LEFT 3
#define TARGETING 4

class Tank
{
public:
	float base_rotation, turret_rotation, barrel_rotation, attack_cd;
	float state_swap_chance, timer, chance_increase;
	glm::vec3 position, forward, damage_position, relative_dmg_pos;
	int hit_points;
	int state;
	bool alive, ai_enabled;
	Tank(glm::vec3 position);
	~Tank();
	void MoveForward(float deltaTimeSeconds);

	void RotateTank(float deltaTimeSeconds);

	void RotateTurret(float deltaTimeSeconds);

	void RotateBarrel(float deltaTimeSeconds);

	ProjectileTank* GetNewProjectile();

	void Update(float deltaTimeSeconds, Tank *player_tank);

	bool CanShoot();

	ProjectileTank* Shoot();

	void TakeDamage(glm::vec3 damage_source);

	ProjectileTank* Target(float deltaTimeSeconds, Tank* player_tank);

	bool TargetHorizontal(float delatTimeSeconds, Tank *player_tank);

	bool TargetVertical(float deltaTimeSeconds, Tank* player_tank);


private:

};
