#pragma once
#include <core/gpu/mesh.h>
#include "MyMeshes.h"
#include "Hexagon.h"

namespace MyObjects
{
	class Turret
	{
	public:
		bool is_alive;
		int type;
		int x, y;
		Mesh* mesh;
		float scale;
		float shoot_timing;
		Turret(int x, int y, int type, Mesh* mesh);
		~Turret();
		void updateState(float deltaTimeSeconds, int can_shoot);
	};
}