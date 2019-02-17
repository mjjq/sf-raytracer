#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "lightSource.hpp"
#include "renderObject.hpp"

class Scene
{
    std::vector<LightSource*> lights{};
    std::vector<RenderObject*> objects{};

    double ambientLightingFactor = 0.0;
    sf::Color backgroundColor = {40,40,40,255};
    int maxReflectionOrder = 1;
public:
    Scene();
    void addLight(LightSource* light);
    void addObject(RenderObject* object);

    void clampColor(sf::Color &color);

    bool testRayAgainstScene(Ray newRay,
                                   int &objIndex,
                                   double &tTemp);

    void setAmbientFactor(double factor);
    void setBackgroundColor(sf::Color color);
    void setReflectionOrder(int order);

    sf::Color raytracePixel(Ray newRay);
};

#endif // SCENE_HPP
