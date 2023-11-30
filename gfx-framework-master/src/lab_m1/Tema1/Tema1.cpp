#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "Tema1.h"
#include "MyMeshes.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    srand(time(NULL));

    game_over = false;
    hit_points = 3;
    credits = 5;
    time_since_last_spawn = 0;
    time_since_last_check = 0;
    coin_spawn_timer = 0;
    spawn_chance = 0;
    spawn_chance_increase = 10;
    selected_turret = -1;
    difficulty_clock = 0;

    squareSide = 125;
    objectSize = (squareSide / 2) * 0.8f;
    star_size = squareSide / 6.2f;
    coin_size = objectSize * 0.9f;
   
    sq_startx = 175;
    sq_starty = 85;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            base_sqx[i][j] = sq_startx + i * squareSide + i * 20;
            base_sqy[i][j] = sq_starty + j * squareSide + j * 20;
            has_turret[i][j] = false;
        }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            has_enemy[i][j] = 0;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        hudx[i] = sq_startx - 40 + i * (squareSide + 40);
        hudy[i] = 720 - sq_starty;
    }

    coin_costs[0] = 1;
    coin_costs[1] = 2;
    coin_costs[2] = 2;
    coin_costs[3] = 3;

    
    glm::vec3 blue_color = glm::vec3(0, 0, 1);
    glm::vec3 orange_color = glm::vec3(1, 0.5f, 0);
    glm::vec3 yellow_color = glm::vec3(1, 1, 0);
    glm::vec3 pink_color = glm::vec3(1, 0, 1);
    glm::vec3 purple_color = glm::vec3(0.5f, 0, 1);


    Mesh* square1 = MyMeshes::CreateSquare("square1", squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square1);

    Mesh* square_hp = MyMeshes::CreateSquare("square_hp", objectSize * 2, glm::vec3(1, 0, 0), true);
    AddMeshToList(square_hp);

    Mesh* blackbox = MyMeshes::CreateSquare("blackbox", squareSide, glm::vec3(0, 0, 0));
    AddMeshToList(blackbox);

    Mesh* rectangle = MyMeshes::CreateSquare("rectangle", squareSide / 2, glm::vec3(1, 0, 0), true);
    AddMeshToList(rectangle);

    Mesh* rhombus_blue = MyMeshes::CreateRhombus("rhombus_blue", objectSize, blue_color);
    AddMeshToList(rhombus_blue);
    turret_meshes[1] = rhombus_blue;

    Mesh* rhombus_orange = MyMeshes::CreateRhombus("rhombus_orange", objectSize, orange_color);
    AddMeshToList(rhombus_orange);
    turret_meshes[0] = rhombus_orange;

    Mesh* rhombus_yellow = MyMeshes::CreateRhombus("rhombus_yellow", objectSize, yellow_color);
    AddMeshToList(rhombus_yellow);
    turret_meshes[2] = rhombus_yellow;

    Mesh* rhombus_purple = MyMeshes::CreateRhombus("rhombus_purple", objectSize, purple_color);
    AddMeshToList(rhombus_purple);
    turret_meshes[3] = rhombus_purple;

    Mesh* hexagon_outer_blue = MyMeshes::CreateHexagon("hexagon_outer_blue", objectSize, blue_color, 2);
    AddMeshToList(hexagon_outer_blue);

    Mesh* hexagon_inner = MyMeshes::CreateHexagon("hexagon_inner", objectSize * 0.70f, glm::vec3(0, 0.75f, 1), 3);
    AddMeshToList(hexagon_inner);

    Mesh* hexagon_outer_orange = MyMeshes::CreateHexagon("hexagon_outer_orange", objectSize, orange_color, 2);
    AddMeshToList(hexagon_outer_orange);

    Mesh* hexagon_outer_yellow = MyMeshes::CreateHexagon("hexagon_outer_yellow", objectSize, yellow_color, 2);
    AddMeshToList(hexagon_outer_yellow);

    Mesh* hexagon_outer_purple = MyMeshes::CreateHexagon("hexagon_outer_purple", objectSize, purple_color, 2);
    AddMeshToList(hexagon_outer_purple);

    Mesh* star_gui = MyMeshes::CreateStar("star_gui", star_size, glm::vec3(0.4f, 0.4f, 0.4f), 2);
    AddMeshToList(star_gui);

    Mesh* star_coin = MyMeshes::CreateStar("star_coin", coin_size, glm::vec3(1, 0, 1), 2);
    AddMeshToList(star_coin);

    Mesh* star_orange = MyMeshes::CreateStar("star_orange", objectSize, orange_color, 2);
    projectile_meshes[0] = star_orange;

    Mesh* star_blue = MyMeshes::CreateStar("star_blue", objectSize, blue_color, 2);
    projectile_meshes[1] = star_blue;

    Mesh* star_yellow = MyMeshes::CreateStar("star_yellow", objectSize, yellow_color, 2);
    projectile_meshes[2] = star_yellow;

    Mesh* star_purple = MyMeshes::CreateStar("star_purple", objectSize, purple_color, 2);
    projectile_meshes[3] = star_purple;

}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.15f, 0.15f, 0.15f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::RenderGUIStars(float startx, float starty, float right_limit, int amount)
{
    float x = startx;
    float y = starty;

    for (int i = 0; i < amount; i++)
    {
        RenderMesh2D(meshes["star_gui"], shaders["VertexColor"], transform2D::Translate(x, y));
        x += 2 * star_size;
        if (x >= right_limit - star_size) 
        {
            y -= 2 * star_size;
            x = startx;
        }
    }
}

void Tema1::Update(float deltaTimeSeconds)
{ 
    if (game_over)
        return;
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], transform2D::Translate(50, base_sqy[0][1]) * transform2D::Scale(1, 6.65));
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            RenderMesh2D(meshes["square1"], shaders["VertexColor"], transform2D::Translate(base_sqx[i][j], base_sqy[i][j]));
        }

    for  (int i = 0; i < 4; i++)
    {
        RenderMesh2D(meshes["blackbox"], shaders["VertexColor"], transform2D::Translate(hudx[i], hudy[i]));
        RenderMesh2D(turret_meshes[i], shaders["VertexColor"], transform2D::Translate(hudx[i], hudy[i]));
        RenderGUIStars(hudx[i] - (squareSide / 2) + star_size , hudy[i] - (squareSide / 2) - star_size, hudx[i] + (squareSide / 2), coin_costs[i]);
    }

    float x = sq_startx - 40 + 4 * (squareSide + 40);
    float y = 720 - sq_starty;
    for (int i = 0; i < hit_points; i++)
    {
        RenderMesh2D(meshes["square_hp"], shaders["VertexColor"], transform2D::Translate(x + i * (2 * objectSize + 40), y));
    }
    RenderGUIStars(x - objectSize + star_size, y - objectSize - star_size - 5, 1280, credits);


    // Enemy spawn logic
    time_since_last_spawn += deltaTimeSeconds;
    time_since_last_check += deltaTimeSeconds;  
    if (time_since_last_check > 1)
    {
        int spawn_roll = rand() % 100;
        if (spawn_roll < spawn_chance)
        {

            int row_roll = spawn_roll % 3;
            int color_roll = spawn_roll % 4;
            MyObjects::Hexagon* new_hexa = NULL;
            switch (color_roll)
            {
            case 0:
                new_hexa = new MyObjects::Hexagon(1280.0f, base_sqy[0][row_roll], 0, meshes["hexagon_outer_orange"], meshes["hexagon_inner"], row_roll);;
                break;

            case 1:
                new_hexa = new MyObjects::Hexagon(1280.0f, base_sqy[0][row_roll], 1, meshes["hexagon_outer_blue"], meshes["hexagon_inner"], row_roll);
                break;

            case 2:
                new_hexa = new MyObjects::Hexagon(1280.0f, base_sqy[0][row_roll], 2, meshes["hexagon_outer_yellow"], meshes["hexagon_inner"], row_roll);
                break;

            case 3:
                new_hexa = new MyObjects::Hexagon(1280.0f, base_sqy[0][row_roll], 3, meshes["hexagon_outer_purple"], meshes["hexagon_inner"], row_roll);
                break;

            default:
                break;
            }

            hex_arr.push_back(new_hexa);
            has_enemy[row_roll][color_roll]++;
            
            std::cout << "Spawned hexa: TIME- " << time_since_last_spawn << endl;
            time_since_last_spawn = 0;
            spawn_chance = 0;
        }
        else
        {
            spawn_chance += spawn_chance_increase * time_since_last_check;
        }
        time_since_last_check = 0;
    }

    difficulty_clock += deltaTimeSeconds;
    if (difficulty_clock > 20) 
    {
        spawn_chance_increase++;
        difficulty_clock = 0;
        cout << "Difficulty increased to " << spawn_chance_increase << endl;
    }
    
    // Hexagon iteration
    for (int i = 0; i < hex_arr.size(); i++)
    {
        hex_arr[i]->updateState(deltaTimeSeconds);
        if (hex_arr[i]->x <= 50 || hex_arr[i]->scale <= 0) {
            if (hex_arr[i]->x <= 50)
            {
                hit_points--;
                if (hit_points == 0) 
                {
                    game_over = true;
                }
                has_enemy[hex_arr[i]->row_number][hex_arr[i]->type]--;
            }           
            delete(hex_arr[i]);
            hex_arr.erase(hex_arr.begin() + i);
            i--;
        }
        else 
        {
            RenderHexagon(hex_arr[i]);
        }
    }

    // Turret iteration
    for (int i = 0; i < turret_arr.size(); i++)
    {
        MyObjects::Turret* curr_turret = turret_arr[i];
        curr_turret->updateState(deltaTimeSeconds, has_enemy[curr_turret->y][curr_turret->type]);

        if (curr_turret->scale <= 0) {
            has_turret[curr_turret->x][curr_turret->y] = false;
            delete(curr_turret);
            turret_arr.erase(turret_arr.begin() + i);
            i--;
        }
        else
        {
            if (curr_turret->shoot_timing > 2) 
            {
                projectile_arr.push_back(new MyObjects::Projectile(base_sqx[curr_turret->x][curr_turret->y],
                    base_sqy[curr_turret->x][curr_turret->y], curr_turret->type, projectile_meshes[curr_turret->type]));
                curr_turret->shoot_timing = 0;
            }
            RenderMesh2D(turret_meshes[curr_turret->type], shaders["VertexColor"],
                transform2D::Translate(base_sqx[curr_turret->x][curr_turret->y], base_sqy[curr_turret->x][curr_turret->y])
                * transform2D::Scale(curr_turret->scale, curr_turret->scale));
        }
    }

    // Projectile iteration
    for (int i = 0; i < projectile_arr.size(); i++)
    {
        MyObjects::Projectile* curr_projectile = projectile_arr[i];
        curr_projectile->updateState(deltaTimeSeconds);
        if (curr_projectile->x >= 1280.0f || !curr_projectile->is_moving) {
            
            delete(curr_projectile);
            projectile_arr.erase(projectile_arr.begin() + i);
            i--;
        }
        else
        {
            RenderMesh2D(projectile_meshes[curr_projectile->type], shaders["VertexColor"], transform2D::Translate(curr_projectile->x, curr_projectile->y)
                * transform2D::Rotate(curr_projectile->angle));
        }
    }

    // Cursor projection render
    if (selected_turret >= 0)
    {
        RenderMesh2D(turret_meshes[selected_turret], shaders["VertexColor"], transform2D::Translate(sceneMousex, sceneMousey));
    }

    // Collision checks
    for (MyObjects::Hexagon* hex : hex_arr)
    {
        for (MyObjects::Turret* turret : turret_arr)
        {
            if (glm::distance(glm::vec3(hex->x, hex->y, 0), glm::vec3(base_sqx[turret->x][turret->y], base_sqy[turret->x][turret->y], 0))
                < 2 * objectSize)
                turret->is_alive = false;
        }

        for (MyObjects::Projectile* projectile : projectile_arr)
        {
            if (projectile->type == hex->type && glm::distance(glm::vec3(hex->x, hex->y, 0), glm::vec3(projectile->x, projectile->y, 0)) < 2 * objectSize && hex->is_moving)
            {
                projectile->is_moving = false;
                hex->hit_points--;
                if (hex->hit_points == 0) {
                    hex->is_moving = false;
                    has_enemy[hex->row_number][hex->type]--;
                }
            }
        }
    }

    coin_spawn_timer += deltaTimeSeconds;
    if (coin_spawn_timer > 10)
    {
        for (size_t i = 0; i < 3; i++)
        {
            float x = rand() % 1280;
            float y = rand() % 720;
            coin_arr.push_back(new MyObjects::Coin(x, y));
        }
        coin_spawn_timer = 0;
    }

    for (int i = 0; i < coin_arr.size(); i++)
    {
        MyObjects::Coin* curr_coin = coin_arr[i];
        if (!curr_coin->available) {

            delete(curr_coin);
            coin_arr.erase(coin_arr.begin() + i);
            i--;
        }
        else
        {
            RenderMesh2D(meshes["star_coin"], shaders["VertexColor"], transform2D::Translate(curr_coin->x, curr_coin->y));
        }
    }
}

void Tema1::RenderHexagon(MyObjects::Hexagon* hexagon)
{
    RenderMesh2D(hexagon->outer_mesh, shaders["VertexColor"], hexagon->state);
    RenderMesh2D(hexagon->inner_mesh, shaders["VertexColor"], hexagon->state);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_R && game_over) 
    {
        game_over = false;
        hit_points = 3;
        credits = 5;
        time_since_last_spawn = 0;
        time_since_last_check = 0;
        difficulty_clock = 0;
        coin_spawn_timer = 0;
        spawn_chance = 0;
        spawn_chance_increase = 10;
        selected_turret = -1;
        hex_arr.clear();
        turret_arr.clear();
        projectile_arr.clear();
        coin_arr.clear();
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                has_turret[i][j] = false;
            }

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                has_enemy[i][j] = 0;
            }
        }
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    sceneMousey = 720 - mouseY;
    sceneMousex = mouseX;
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    int sceneY = 720 - mouseY;
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2) // click stanga
    {
        for (int i = 0; i < 4; i++)
        {
            if (mouseX > (hudx[i] - squareSide / 2) &&
                mouseX < (hudx[i] + squareSide / 2) &&
                sceneY >(hudy[i] - squareSide / 2) &&
                sceneY < (hudy[i] + squareSide / 2) &&
                credits >= coin_costs[i])
            {
                std::cout << "Pressed on " << i << endl;
                selected_turret = i;
            }
        }

        for (MyObjects::Coin* curr_coin : coin_arr)
        {
            if (mouseX > (curr_coin->x - coin_size) &&
                mouseX < (curr_coin->x + coin_size) &&
                sceneY >(curr_coin->y - coin_size) &&
                sceneY < (curr_coin->y + coin_size))
            {
                curr_coin->available = false;
                credits++;
                break;
            }
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_3)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                if (mouseX > (base_sqx[i][j] - squareSide / 2) &&
                    mouseX < (base_sqx[i][j] + squareSide / 2) &&
                    sceneY >(base_sqy[i][j] - squareSide / 2) &&
                    sceneY < (base_sqy[i][j] + squareSide / 2) &&
                    has_turret[i][j])
                {
                    for (int k = 0; k < turret_arr.size(); k++)
                    {
                        if (turret_arr[k]->x == i && (turret_arr[k]->y == j))
                        {
                            turret_arr[k]->is_alive = false;
                        }
                    }
                    std::cout << "Killed turret on " << i << j << endl;
                }
            }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    int sceneY = 720 - mouseY;
    // Add mouse button release event
    if (button == GLFW_MOUSE_BUTTON_2) // click stanga
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                if (mouseX > (base_sqx[i][j] - squareSide / 2) &&
                    mouseX < (base_sqx[i][j] + squareSide / 2) &&
                    sceneY > (base_sqy[i][j] - squareSide / 2) &&
                    sceneY < (base_sqy[i][j] + squareSide / 2) &&
                    !has_turret[i][j] && selected_turret >= 0)
                {
                    turret_arr.push_back(new MyObjects::Turret(i, j, selected_turret, turret_meshes[selected_turret]));
                    credits -= coin_costs[selected_turret];
                    std::cout << "Spawned turret on " << i << j << endl;
                    has_turret[i][j] = true;
                }
            }
        selected_turret = -1;
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
