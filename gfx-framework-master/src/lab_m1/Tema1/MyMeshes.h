#pragma once
#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace MyMeshes
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRhombus(const std::string& name, float length, glm::vec3 color);
    Mesh* CreateHexagon(const std::string& name, float length, glm::vec3 color, float depth);
    Mesh* CreateStar(const std::string& name, float length, glm::vec3 color, float depth);
}