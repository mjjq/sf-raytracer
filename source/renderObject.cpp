#include "renderObject.hpp"
#include <cmath>
#include <iostream>

RenderObject::RenderObject(Vec3d _position,
                           Vec3d _size,
                           MaterialProperties _mPs) :
                               position{_position},
                               matProperties{_mPs}
{
    radius = sqrt(_size.dot(_size));
}

RenderObject::RenderObject()
{

}

bool RenderObject::intersects(Ray ray, double &t)
{
    Vec3d relPos = ray.o - position;

    //a=1 because ray direction is normalised
    double b = 2.0*ray.d.dot(relPos);
    double c = relPos.dot(relPos) - radius*radius;

    double discriminant = b*b - 4.0*c;

    if(discriminant < 0.0)
        return false;

    double sqrtDisc = sqrt(discriminant);
    double t0 = (-b - sqrtDisc)/2.0;
    double t1 = (-b + sqrtDisc)/2.0;

    t = (t0 < t1)? t0 : t1;
    return true;
}

Vec3d RenderObject::getIntNormal(Vec3d intPoint)
{
    return (intPoint - position)/radius;
}

Plane::Plane(Vec3d _position,
           Vec3d _size,
           MaterialProperties _mPs) : RenderObject(_position, _size, _mPs)
{
    normal = Vec3d::norm(_size);
}

Plane::Plane() : RenderObject()
{

}

bool Plane::intersects(Ray ray, double &t)
{
    double epsilon = 1e-15;

    double a = ray.d.dot(normal);
    if(std::abs(a) > epsilon )
    {
        t = (position-ray.o).dot(normal) / a;
        return true;
    }
    return false;
}

Vec3d Plane::getIntNormal(Vec3d intPoint)
{
    return normal;
}
