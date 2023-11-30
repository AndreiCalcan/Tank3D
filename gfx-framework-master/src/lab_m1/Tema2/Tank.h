#pragma once
#include "components/simple_scene.h"
#include "ProjectileTank.h"

#define MESH_SCALE 0.15f
#define BALL_OFFSET glm::vec3(0, 2.2073f, -5.35f) * MESH_SCALE
#define BARREL_OFFSET glm::vec3(0, 2.2667f, -0.2f) * MESH_SCALE
#define TURRET_OFFSET MESH_SCALE * glm::vec3(0, 2.0478f, 1.2675f)
#define TANK_RADIUS MESH_SCALE * 4.4f
#define PROJECTILE_RADIUS MESH_SCALE * 0.2f

class Tank
{
public:
	float base_rotation, turret_rotation, barrel_rotation, attack_cd;
	glm::vec3 position, forward, damage_position;
	int hit_points;
	Tank(glm::vec3 position);
	~Tank();
	void MoveForward(float deltaTimeSeconds);

	void RotateTank(float deltaTimeSeconds);

	void RotateTurret(float deltaTimeSeconds);

	void RotateBarrel(float deltaTimeSeconds);

	ProjectileTank* GetNewProjectile();

	void Update(float deltaTimeSeconds);

	bool CanShoot();

	ProjectileTank* Shoot();

	void TakeDamage(glm::vec3 damage_source);


private:

};
