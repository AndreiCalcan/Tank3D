#pragma once
#include <core/gpu/mesh.h>
#include "MyMeshes.h"


namespace MyObjects 
{
	class Hexagon
	{
	public:
		int type;
		float x, y;
		int row_number;
		Mesh *outer_mesh, *inner_mesh;
		bool is_moving;
		int hit_points;
		float scale;
		glm::mat3 state;
		Hexagon(float x, float y, int type, Mesh* outer_mesh, Mesh* inner_mesh, int row_number);
		~Hexagon();
		void updateState(float deltaTimeSeconds);
	};
}