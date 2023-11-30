#pragma once
#include <core/gpu/mesh.h>
#include "MyMeshes.h"
namespace MyObjects
{
	class Projectile
	{
	public:
		int type;
		float angle;
		float x, y;
		Mesh* mesh;
		bool is_moving;
		Projectile(float x, float y, int type, Mesh* mesh);
		~Projectile();
		void updateState(float deltaTimeSeconds);
	};

	class Coin
	{
	public:
		float x, y;
		bool available;
		Coin(float x, float y);
		~Coin();
	};
}