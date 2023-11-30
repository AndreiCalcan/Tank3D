#include "Hexagon.h"
#include <components/simple_scene.h>
#include "transform2D.h"

using namespace MyObjects;

Hexagon::Hexagon(float x, float y, int type, Mesh* outer_mesh, Mesh* inner_mesh, int row_number)
{
	this->x = x;
	this->y = y;
	this->type = type;
	this->outer_mesh = outer_mesh;
	this->inner_mesh = inner_mesh;
	this->hit_points = 3;
	this->is_moving = true;
	this->scale = 1;
	this->state = transform2D::Translate(x, y);
	this->row_number = row_number;
}

Hexagon::~Hexagon()
{
}

void Hexagon::updateState(float deltaTimeSeconds)
{
	if (is_moving)
	{
		x -= 50 * deltaTimeSeconds;
		state = transform2D::Translate(x, y);
	}
	else
	{
		scale -= deltaTimeSeconds / 2;
		state = transform2D::Translate(x, y) * transform2D::Scale(scale, scale);
	}
}
	