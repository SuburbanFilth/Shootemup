#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Systems/Physics/PhysicsSystem.hpp"
#include "Systems/Animation/AnimationSystem.hpp"
#include "Systems/Input/InputSystem.hpp"
#include "Systems/Rendering/RenderingSystem.hpp"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Entity;
class Game
{
    public:
        Game(int width,int height);

        void Init();
        void InitConfiguration();
        void InitScene(std::string filename, std::vector<std::shared_ptr<Entity>>& entities);

        void Run();
        void Update(float deltaTime);

        std::pair<unsigned int, unsigned int> SetupBuffers(float* data, int size, bool animated);

        // data
        GameState state;

        int                         width;
        int                         height;
        GLFWwindow*                 window;

        // Entities
        std::vector<std::shared_ptr<Entity>> entities;

        // SYSTEMS
        InputSystem                 inputSystem;
        PhysicsSystem               physicsSystem;
        AnimationSystem             animationSystem;
        RenderingSystem             renderingSystem;
};