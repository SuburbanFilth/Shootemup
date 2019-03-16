#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Collider.hpp"

class Triangle : public Collider
{
    public:
        Triangle(   glm::vec3               center, 
                    glm::vec3               normal, 
                    std::vector<glm::vec3>  points, 
                    ColliderType            colliderType,
                    DynamicType             dynamicType);
        std::vector<glm::vec3> GetPoints();
        glm::vec3 normal;
        std::vector<glm::vec3> points;
    private:
};