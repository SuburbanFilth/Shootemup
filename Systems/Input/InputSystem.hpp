#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>

class Entity;
class InputSystem
{
    public:
        InputSystem();
        ~InputSystem();

        void Update(GLFWwindow* window, std::vector<std::shared_ptr<Entity>>& entities);
    private:
        std::uint32_t primaryBitset;
};  