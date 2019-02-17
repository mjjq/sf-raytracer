#ifndef RENDER_OBJ_HPP
#define RENDER_OBJ_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "mathUtils.hpp"
#include "materials.hpp"


class RenderObject
{
protected:
    Vec3d position = Vec3d(0.0, 0.0, 0.0);
    float radius = 100.0f;
public:
    RenderObject(Vec3d _position, Vec3d _size, MaterialProperties _mPs);
    RenderObject();

    MaterialProperties matProperties;

    virtual bool intersects(Ray ray, double &t);
    virtual Vec3d getIntNormal(Vec3d intPoint);
};


class Plane : public RenderObject
{
    Vec3d normal = {0.0, 0.0, 1.0};
public:
    Plane(Vec3d _position, Vec3d _size, MaterialProperties _mPs);
    Plane();
    bool intersects(Ray ray, double &t);
    Vec3d getIntNormal(Vec3d intPoint);
};
#endif // RENDER_OBJ_HPP
