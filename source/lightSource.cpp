#include "lightSource.hpp"

#include <cmath>

LightSource::LightSource(Vec3d pos, Vec3d dir, sf::Color color) :
    LightSourcePos{pos}, LightSourceDir{Vec3d::norm(dir)}, lightColor{color}
{
    targetDistance = sqrt(pos.dot(pos));
}

void LightSource::setPos(Vec3d pos)
{
    LightSourcePos = pos;
}

void LightSource::setDir(Vec3d dir)
{
    LightSourceDir = Vec3d::norm(dir);
}

Vec3d LightSource::getPos()
{
    return LightSourcePos;
}

Vec3d LightSource::getDir()
{
    return LightSourceDir;
}

sf::Color LightSource::getColor()
{
    return lightColor;
}

void LightSource::rotateLightSource(double deltaPhi,
                         double deltaTheta)
{
    phi += deltaPhi;
    theta += deltaTheta;

    double x = targetDistance*sin(theta)*cos(phi);
    double y = targetDistance*sin(theta)*sin(phi);
    double z = targetDistance*cos(theta);

    Vec3d newPos(x,y,z);

    LightSourcePos = newPos;
    LightSourceDir = Vec3d::norm(newPos)*-1.0;
}

