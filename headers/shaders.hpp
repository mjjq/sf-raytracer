#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <SFML/Graphics.hpp>
#include "mathUtils.hpp"
#include "lightSource.hpp"

class Shaders
{
public:
    static bool testRayAgainstScene(Ray newRay,
                                   int &objIndex,
                                   double &tTemp);
    static sf::Color lambertShader(Vec3d pi,
                                   RenderObject* object,
                                   LightSource* light);

    static sf::Color specular(Vec3d pi,
                            RenderObject* object,
                            LightSource* light,
                            Ray cameraRay);

    static Vec3d calcReflectionVector(Vec3d incidentDir, Vec3d normalDir);

    static bool isLightObstructed(Ray lightRay,
                                  std::vector<RenderObject*> &sceneObjects,
                                  int ignoreObject);
};

#endif // SHADERS_HPP
