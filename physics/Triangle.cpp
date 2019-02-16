#include "Triangle.hpp"

Triangle::Triangle(glm::vec3 center) : BoundingVolume(center)
{

}

bool Triangle::CheckCollision(Triangle triangle)
{
    // https://cis.temple.edu/~lakaemper/courses/cis350_2004/etc/moeller_triangle.pdf
}

bool Triangle::CheckCollision(Sphere sphere)
{
    // if the distance between the plane of the triangle and the center of the 
    // sphere is less than the radius, then they intersect.
    
}

bool Triangle::CheckCollision(BoundingBox box)
{

}