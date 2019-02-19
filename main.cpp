#include <iostream>

// #include "Game.hpp"
#include "physics/Grid.hpp"
#include "physics/Triangle.hpp"

int main(int argc, char *argv[])
{
//     Game game(800,600);
//     // Start Game within Menu Statea
//     game.state = GAME_ACTIVE;
// //    
//     game.Run();
    Grid grid = Grid(200.f, 10.f);
    std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(glm::vec3(100.f,0.f,100.f), glm::vec3(1.0f,1.f,1.f), false);
    grid.Insert(triangle);
    for (int i = 0; i < 20000; i++)
    {
        std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(glm::vec3(rand() % 200, rand() % 200, rand() % 200),glm::vec3(1.0f,1.f,1.f), false);
        grid.Insert(triangle);
    }
    grid.CheckCollisions();
    return 0;
}