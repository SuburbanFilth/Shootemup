#pragma once

#include "BoundingVolume.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

class BoundingBox : public BoundingVolume
{
    public:

        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(BoundingBox box);

    private:

};