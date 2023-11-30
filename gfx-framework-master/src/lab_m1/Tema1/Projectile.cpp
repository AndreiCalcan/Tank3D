#include "Projectile.h"
#include <components/simple_scene.h>
#include "transform2D.h"

using namespace MyObjects;

Projectile::Projectile(float x, float y, int type, Mesh* mesh)
{
	this->x = x;
	this->y = y;
	this->type = type;
	this->mesh = mesh;
	this->is_moving = true;
	this->angle = 0;
}

Projectile::~Projectile()
{
}

void Projectile::updateState(float deltaTimeSeconds) 
{
	x += 150 * deltaTimeSeconds;
	angle -= 6 * deltaTimeSeconds;
}

Coin::Coin(float x, float y)
{
	this->x = x;
	this->y = y;
	this->available = true;
}

Coin::~Coin()
{
}
