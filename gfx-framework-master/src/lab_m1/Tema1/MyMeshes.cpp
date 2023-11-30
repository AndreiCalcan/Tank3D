#include "MyMeshes.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <math.h>

Mesh* MyMeshes::CreateSquare(
    const std::string& name,
    float length,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(length / 2, length / 2, 0), color),
        VertexFormat(glm::vec3(length / 2, - length / 2, 0), color),
        VertexFormat(glm::vec3(- length / 2, - length / 2, 0), color),
        VertexFormat(glm::vec3(- length / 2, length / 2, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* MyMeshes::CreateRhombus(const std::string& name, float length, glm::vec3 color) 
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, length, 1), color),
        VertexFormat(glm::vec3(0, -length, 1), color),
        VertexFormat(glm::vec3(-length / 2, 0, 1), color),
        VertexFormat(glm::vec3(length / 2, length * 0.15, 1), color),
        VertexFormat(glm::vec3(length, length * 0.15, 1), color),
        VertexFormat(glm::vec3(length, length * -0.15, 1), color),
        VertexFormat(glm::vec3(length / 2, length * -0.15, 1), color)    
    };

    Mesh* rombus = new Mesh(name);
    std::vector<unsigned int> indices = 
    { 
     0, 2, 3, 
     3, 2, 6,
     6, 2, 1,
     4, 3, 5,
     5, 3, 6
    };

    rombus->InitFromData(vertices, indices);
    return rombus;
}

Mesh* MyMeshes::CreateHexagon(const std::string& name, float length, glm::vec3 color, float depth)
{
    float angle = M_PI/3;


    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, depth), color),
        VertexFormat(glm::vec3(cos(angle) * length, sin(angle) * length, depth), color),
        VertexFormat(glm::vec3(cos(2 * angle) * length, sin(2 * angle) * length, depth), color),
        VertexFormat(glm::vec3(cos(3 * angle) * length, sin(3 * angle) * length, depth), color),
        VertexFormat(glm::vec3(cos(4 * angle) * length, sin(4 * angle) * length, depth), color),
        VertexFormat(glm::vec3(cos(5 * angle) * length, sin(5 * angle) * length, depth), color),
        VertexFormat(glm::vec3(cos(6 * angle) * length, sin(6 * angle) * length, depth), color)
    };

    Mesh* hexagon = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 1};

    hexagon->SetDrawMode(GL_TRIANGLE_FAN);
    hexagon->InitFromData(vertices, indices);
    return hexagon;
}

Mesh* MyMeshes::CreateStar(const std::string& name, float length, glm::vec3 color, float depth)
{
    float angle = (2 * M_PI) / 5;
    float halfpi = M_PI / 2;
    float inner_len = length / 2.5f;
    float inner_offset = halfpi + M_PI / 5;


    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, depth), color),
        VertexFormat(glm::vec3(cos(halfpi) * length, sin(halfpi) * length, depth), color),
        VertexFormat(glm::vec3(cos(angle + halfpi) * length, sin(angle + halfpi) * length, depth), color),
        VertexFormat(glm::vec3(cos(2 * angle + halfpi) * length, sin(2 * angle + halfpi) * length, depth), color),
        VertexFormat(glm::vec3(cos(3 * angle + halfpi) * length, sin(3 * angle + halfpi) * length, depth), color),
        VertexFormat(glm::vec3(cos(4 * angle + halfpi) * length, sin(4 * angle + halfpi) * length, depth), color),
        VertexFormat(glm::vec3(cos(inner_offset) * inner_len, sin(inner_offset) * inner_len, depth), color),
        VertexFormat(glm::vec3(cos(angle + inner_offset) * inner_len, sin(angle + inner_offset) * inner_len, depth), color),
        VertexFormat(glm::vec3(cos(2 * angle + inner_offset) * inner_len, sin(2 * angle + inner_offset) * inner_len, depth), color),
        VertexFormat(glm::vec3(cos(3 * angle + inner_offset) * inner_len, sin(3 * angle + inner_offset) * inner_len, depth), color),
        VertexFormat(glm::vec3(cos(4 * angle + inner_offset) * inner_len, sin(4 * angle + inner_offset) * inner_len, depth), color),
    };

    Mesh* hexagon = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 6, 2, 7, 3, 8, 4, 9, 5, 10, 1 };

    hexagon->SetDrawMode(GL_TRIANGLE_FAN);
    hexagon->InitFromData(vertices, indices);
    return hexagon;
}