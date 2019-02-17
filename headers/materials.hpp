#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include <SFML/Graphics.hpp>

struct MaterialProperties
{
    sf::Color diffuseColor;
    double diffuseCoef;
    int specularPower;
    double specularCoef;
    double reflectivity;
};

namespace Material
{
    const MaterialProperties whiteChalk = {sf::Color::White, 1.0, 1, 0.0, 0.0};
    const MaterialProperties whiteCeramic = {sf::Color::White, 1.0, 16, 0.1, 0.08};
}

#endif // MATERIALS_HPP
