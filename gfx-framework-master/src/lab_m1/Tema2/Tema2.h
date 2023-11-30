#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "Tank.h"


struct Building {
    glm::vec3 position;
    glm::vec3 scale;
};

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        glm::vec3 Collide(glm::vec3 sphere_pos, float sphere_radius, Building* box);

        void Init() override;

     private:
        void FrameStart() override;
        void RenderTank(Tank* tank);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 meshColor, Tank* tank);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     public:
        glm::vec3 camera_direction;
        Tank* player_tank;
        implemented::MyCamera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget, follow_tank, free_look;
        float fov, right, camera_distance;
        std::vector<struct Tank*> tank_arr;
        std::vector<struct Building*> building_arr;
        std::vector<struct ProjectileTank*> projectile_arr;
        float max_building_scale, min_building_scale;

        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
