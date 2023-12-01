#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


glm::vec3 Tema2::Collide(glm::vec3 sphere_pos, float sphere_radius, Building* box)
{
    float maxX = box->position.x + box->scale.x;
    float minX = box->position.x - box->scale.x;
    float maxY = box->position.y + box->scale.y;
    float minY = box->position.y - box->scale.y;
    float maxZ = box->position.z + box->scale.z;
    float minZ = box->position.z - box->scale.z;

    glm::vec3 closest_point;
    
    closest_point.x = max(minX, min(sphere_pos.x, maxX));
    closest_point.y = max(minY, min(sphere_pos.y, maxY));
    closest_point.z = max(minZ, min(sphere_pos.z, maxZ));

    float distance = glm::distance(sphere_pos, closest_point);
    glm::vec3 direction = glm::normalize(closest_point - sphere_pos);
    if (distance < sphere_radius)
    {
        return sphere_pos - direction * (sphere_radius - distance);
    }
    return sphere_pos;
}

void Tema2::Init()
{
    window->DisablePointer();
    renderCameraTarget = false;
    fov = 90;
    right = 3.0f;
    follow_tank = true;
    free_look = false;
    player_tank = new Tank(glm::vec3(0, 0, 0));
    tank_arr.push_back(player_tank);
    player_tank->ai_enabled = false;
    srand(time(NULL));

    camera = new implemented::MyCamera();
    camera->Set(glm::vec3(0, 1, 2.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera_direction = glm::normalize(glm::vec3(0, 1, 2.5f));
    camera_distance = glm::distance(glm::vec3(0, 1, 2.5f), glm::vec3(0, 0, 0));

    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Corp.ply");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Turela.ply");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senila");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Senila.ply");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teava");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Teava.ply");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("building");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "MyBox.ply");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Projectile.ply");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);


    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    max_building_scale = 3.5;
    min_building_scale = 2.0f;
    int deltaScale = (max_building_scale - min_building_scale) * 1000;

    // Building spawn
    int building_nr = rand() % 8 + 8;
    for (int i = 0; i < building_nr; i++)
    {
        float x = (rand() % 5000) / 100.0f - 25;
        float z = (rand() % 5000) / 100.0f - 25;

        if (x > -max_building_scale && x < max_building_scale)
        {
            if (x < 0)
                x -= max_building_scale;
            else
                x += max_building_scale;
        }

        float scaleX = (rand() % deltaScale) / 1000.0f + min_building_scale;
        float scaleZ = (rand() % deltaScale) / 1000.0f + min_building_scale;
        float scaleY = (rand() % 3500) / 1000.0f + 5;

        Building* new_building = new Building();
        new_building->position = glm::vec3(x, 0, z);
        new_building->scale = glm::vec3(scaleX, scaleY, scaleZ);
        building_arr.push_back(new_building);
    }

    // Enemy spawn
    int enemy_number = 3;
    for (int i = 0; i < enemy_number; i++)
    {
        float x = (rand() % 5000) / 100.0f - 25;
        float z = (rand() % 5000) / 100.0f - 25;
        Tank* new_tank = new Tank(glm::vec3(x, 0, z));

        bool collides = false;
        for (int j = 0; j < building_arr.size(); j++)
        {
            Building* curr_building = building_arr[j];
            glm::vec3 new_position = Collide(new_tank->position, TANK_RADIUS, curr_building);
            if (new_position != new_tank->position) 
            {
                collides = true;
                break;
            }
        }
        if (collides)
        {
            cout << "Rerolled enemy spawn..." << endl;
            i--;
            continue;
        }
        tank_arr.push_back(new_tank);
    }
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.08f, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderTank(Tank* tank)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank->position);
        modelMatrix = glm::rotate(modelMatrix, tank->base_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, MESH_SCALE * glm::vec3(0, 0, 0));
        tank->damage_position = glm::vec3(modelMatrix * glm::vec4(tank->relative_dmg_pos, 1));
        modelMatrix = glm::scale(modelMatrix, MESH_SCALE * glm::vec3(1, 1, 1));
        RenderSimpleMesh(meshes["corp"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0.25f, 0.05f), tank);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank->damage_position);
        modelMatrix = glm::scale(modelMatrix, MESH_SCALE * glm::vec3(1, 1, 1));
        RenderSimpleMesh(meshes["projectile"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0.25f, 0.05f), NULL);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank->position);
        modelMatrix = glm::rotate(modelMatrix, tank->base_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, TURRET_OFFSET);
        modelMatrix = glm::rotate(modelMatrix, tank->turret_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, MESH_SCALE * glm::vec3(0.87f, 0.87f, 0.87f));

        RenderSimpleMesh(meshes["turela"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0.5, 0.09f), tank);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank->position);
        modelMatrix = glm::rotate(modelMatrix, tank->base_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, TURRET_OFFSET);
        modelMatrix = glm::rotate(modelMatrix, tank->turret_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, BARREL_OFFSET - TURRET_OFFSET);
        modelMatrix = glm::rotate(modelMatrix, tank->barrel_rotation, glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, MESH_SCALE * glm::vec3(1, 1, 1));

        RenderSimpleMesh(meshes["teava"], shaders["TemaShader"], modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f), tank);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank->position);
        modelMatrix = glm::rotate(modelMatrix, tank->base_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, MESH_SCALE * glm::vec3(2.5f, 0.58f, 0));
        //modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, MESH_SCALE * glm::vec3(1, 1, 1));

        RenderSimpleMesh(meshes["senila"], shaders["TemaShader"], modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f), tank);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank->position);
        modelMatrix = glm::rotate(modelMatrix, tank->base_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, MESH_SCALE * glm::vec3(-2.5f, 0.58f, 0));
        //modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, MESH_SCALE * glm::vec3(1, 1, 1));

        RenderSimpleMesh(meshes["senila"], shaders["TemaShader"], modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f), tank);
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    // Camera follow
    if (follow_tank)
        camera->Set(player_tank->position + camera_distance * camera_direction, player_tank->position, glm::vec3(0, 1, 0));

    // Tank render and update
    for (int i = 0; i < tank_arr.size(); i++)
    {
        tank_arr[i]->Update(deltaTimeSeconds);
        RenderTank(tank_arr[i]);
    }


    // Plane render
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.1f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
        RenderSimpleMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 1, 0.18f), NULL);
    }

    // Building render
    for (int i = 0; i < building_arr.size(); i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, building_arr[i]->position);
        modelMatrix = glm::scale(modelMatrix, building_arr[i]->scale);
        RenderSimpleMesh(meshes["building"], shaders["TemaShader"], modelMatrix, glm::vec3(0.4f, 0.4f, 0.4f), NULL);
    }

    // Projectile render and update
    for (int i = 0; i < projectile_arr.size(); i++)
    {
        ProjectileTank* curr_projectile = projectile_arr[i];
        curr_projectile->UpdatePosition(deltaTimeSeconds);
        if (!curr_projectile->alive)
        {
            delete(curr_projectile);
            projectile_arr.erase(projectile_arr.begin() + i);
            i--;
            continue;
        }
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, projectile_arr[i]->position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(MESH_SCALE));
        RenderSimpleMesh(meshes["projectile"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0, 0), NULL);
    }

    // Tank collisions
    for (int i = 0; i < tank_arr.size(); i++)
    {
        Tank* curr_tank = tank_arr[i];
        // Tank - Tank
        for (int j = 0; j < tank_arr.size(); j++)
        {
            Tank *other_tank = tank_arr[j];
            float dist = glm::distance(curr_tank->position, other_tank->position);
            float displacement = 2 * TANK_RADIUS - dist;
            if (i != j && displacement > 0)
            {
                glm::vec3 dif = other_tank->position - curr_tank->position;
                glm::vec3 displacement_dir = displacement * glm::normalize(dif);
                curr_tank->position -= displacement_dir * 0.5f;
                other_tank->position += displacement_dir * 0.5f;
            }
        }

        // Tank - Projectile
        for (int j = 0; j < projectile_arr.size(); j++)
        {
            ProjectileTank* curr_projectile = projectile_arr[j];
            float dist = glm::distance(curr_tank->position, curr_projectile->position);
            float displacement = TANK_RADIUS + PROJECTILE_RADIUS - dist;
            if (displacement > 0)
            {
                curr_tank->TakeDamage(curr_projectile->position);
                curr_projectile->alive = false;
            }
        }

        // Tank - Building
        for (int j = 0; j < building_arr.size(); j++)
        {
            Building* curr_building = building_arr[j];
            curr_tank->position = Collide(curr_tank->position, TANK_RADIUS, curr_building);
        }
    }

    // Projectile - Building
    for (int i = 0; i < projectile_arr.size(); i++)
    {
        ProjectileTank* curr_projectile = projectile_arr[i];
        for (int j = 0; j < building_arr.size(); j++)
        {
            Building* curr_building = building_arr[j];
            glm::vec3 new_pos = Collide(curr_projectile->position, PROJECTILE_RADIUS, curr_building);
            if (new_pos != curr_projectile->position)
            {
                curr_projectile->alive = false;
            }
        }
    }

}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 meshColor, Tank *tank)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    int location;
    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    location = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
    location = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    location = glGetUniformLocation(shader->program, "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    location = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(location, 1, glm::value_ptr(meshColor));

    float hp;
    glm::vec3 damage_source;
    if (tank)
    {
        hp = 1 - (3 - tank->hit_points) * 0.25f;
        damage_source = tank->damage_position;
    }
    else
    {
        hp = 1;
        damage_source = glm::vec3(0, 0, 0);
    }

    location = glGetUniformLocation(shader->program, "HP");
    glUniform1f(location, hp);

    location = glGetUniformLocation(shader->program, "Damage_source");
    glUniform3fv(location, 1, glm::value_ptr(damage_source));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        follow_tank = false;
    }
    else 
    {
        if (!follow_tank)
        {
            follow_tank = true;
            window->DisablePointer();
            return;
        }
        
        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            player_tank->MoveForward(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera forward
            player_tank->MoveForward(-deltaTime);;
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera forward
            player_tank->RotateTank(deltaTime);
            camera_direction = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), deltaTime, glm::vec3(0, 1, 0)) * glm::vec4(camera_direction, 1)));
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera forward
            player_tank->RotateTank(-deltaTime);
            camera_direction = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), -deltaTime, glm::vec3(0, 1, 0)) * glm::vec4(camera_direction, 1)));
        }
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.

    if (window->KeyHold(GLFW_KEY_M)) {
        fov += 2.0 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_N)) {
        fov -= 2.0 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_B)) {
        right += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_V)) {
        right -= deltaTime;
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        for (size_t i = 0; i < tank_arr.size(); i++)
        {
            cout << tank_arr[i]->position << endl;
        }
    }
    // TODO(student): Switch projections

    if (key == GLFW_KEY_O)
    {
        //projectionMatrix = glm::ortho(-3.0f, right, -3.0f, 3.0f, 0.1f, 10.0f);
        player_tank->hit_points--;
    }

    if (key == GLFW_KEY_P)
    {
        //projectionMatrix = glm::perspective(fov, 4.0f/3.0f, 0.1f, 10.0f);
        player_tank->hit_points++;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        free_look = true;
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
    }
    else
    {
        if (free_look)
        {
            free_look = false;
            return;
        }
        float sensitivityX = 0.001f;
        float sensitivityY = 0.0005f;
        player_tank->RotateTurret(- sensitivityX * deltaX);
        player_tank->RotateBarrel(- sensitivityY * deltaY);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2 && player_tank->CanShoot()) // click stanga
    {
        projectile_arr.push_back(player_tank->Shoot());
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
