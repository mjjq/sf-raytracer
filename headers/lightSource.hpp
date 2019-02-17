#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

#include <SFML/Graphics.hpp>
#include "renderObject.hpp"

class LightSource
{
    Vec3d LightSourcePos;
    Vec3d LightSourceDir;

    double phi = 0.0;
    double theta = 0.0;
    double targetDistance = 200.0f;

    sf::Color lightColor = sf::Color::Blue;

public:
    LightSource(Vec3d pos, Vec3d dir, sf::Color color);

    void setPos(Vec3d pos);
    void setDir(Vec3d dir);

    Vec3d getPos();
    Vec3d getDir();

    sf::Color getColor();

    void rotateLightSource(double deltaPhi,
                    double deltaTheta);

};

#endif // LIGHTSOURCE_HPP
