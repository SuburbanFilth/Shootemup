#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component.hpp"

enum DynamicType
{
    Static,
    Dynamic,
    WithPhysics
};

class Collider;
class PhysicsComponent : public Component
{
    public:
        PhysicsComponent( float mass, 
                        glm::vec3 position,
                        glm::quat orientation,
                        glm::mat3 inertiaTensor,
                        DynamicType type);
        ~PhysicsComponent();

        // linear components;
        float     inverseMass;
        glm::vec3 acceleration;
        glm::vec3 velocity;
        glm::vec3 position;
        // used to add all forces that need to be applied to the object.
        glm::vec3 forceAccumulator;

        // angular components
        glm::vec3 angularAcc;
        glm::vec3 angularVel;
        glm::quat orientation;
        glm::vec3 torqueAccumulator;
        glm::mat3 invInertiaTensor;
        glm::mat3 invInertiaTensorLocal;

        // Collider info
        std::vector<std::shared_ptr<Collider>> colliders;

        DynamicType dynamicType;
};