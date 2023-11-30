#pragma once
#pragma once

#include "components/simple_scene.h"
#include "Hexagon.h"
#include "Turret.h"
#include "Projectile.h"
#include <vector>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderHexagon(MyObjects::Hexagon *hexagon);
        void RenderGUIStars(float x, float y, float right_limit, int amount);

    protected:
        std::vector<MyObjects::Hexagon *>hex_arr;
        std::vector<MyObjects::Turret*>turret_arr;
        std::vector<MyObjects::Projectile*>projectile_arr;
        std::vector<MyObjects::Coin*>coin_arr;
        Mesh* turret_meshes[4];
        Mesh* projectile_meshes[4];
        float squareSide, objectSize;
        float star_size, coin_size;
        float base_sqx[3][3], base_sqy[3][3], has_turret[3][3];
        float hudx[4], hudy[4];
        int coin_costs[4];
        int has_enemy[3][4];
        bool game_over;
        float sq_startx, sq_starty;
        int hit_points, credits, selected_turret;
        float time_since_last_spawn, time_since_last_check, difficulty_clock;
        float spawn_chance, spawn_chance_increase;
        float coin_spawn_timer;
        int sceneMousex, sceneMousey;
        // float cx, cy;
        // glm::mat3 modelMatrix;
        // float translateX, translateY;
        // float scaleX, scaleY;
        // float angularStep;
        // int direction, sdirection;

        // TODO(student): If you need any other class variables, define them here.

    };
}