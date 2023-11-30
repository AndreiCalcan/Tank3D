#include "Turret.h"
#include <components/simple_scene.h>

using namespace MyObjects;

Turret::Turret(int x, int y, int type, Mesh* mesh)
{
	this->x = x;
	this->y = y;
	this->type = type;
	this->mesh = mesh;
	this->is_alive = true;
	this->scale = 1;
	this->shoot_timing = 0;
}

Turret::~Turret()
{
}

void Turret::updateState(float deltaTimeSeconds, int can_shoot)
{
	if (can_shoot > 0 && is_alive) {
		shoot_timing += deltaTimeSeconds;
	}
	if (!is_alive)
	{
		scale -= deltaTimeSeconds;
	}
}