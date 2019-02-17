#include "scene.hpp"
#include "shaders.hpp"
#include "mathUtils.hpp"

Scene::Scene() {}

void Scene::addLight(LightSource* light)
{
    lights.emplace_back(std::move(light));
}

void Scene::addObject(RenderObject* object)
{
    objects.emplace_back(std::move(object));
}

void Scene::clampColor(sf::Color &color)
{
    if(color.r > 255) color.r = 255;
    if(color.g > 255) color.g = 255;
    if(color.b > 255) color.b = 255;
    if(color.a > 255) color.a = 255;

    if(color.r < 0) color.r = 0;
    if(color.g < 0) color.g = 0;
    if(color.b < 0) color.b = 0;
    if(color.a < 0) color.a = 0;

}

bool Scene::testRayAgainstScene(Ray newRay,
                                   int &objIndex,
                                   double &tTemp)
{
    double tInt = 1e+15;
    bool foundIntersection = false;

    for(int i=0; i<(int)objects.size(); ++i)
    {
        double temp = tInt;
        if(objects[i]->intersects(newRay, temp))
            if(temp <= tInt && temp > 0.0)
            {
                foundIntersection = true;
                tInt = temp;
                objIndex = i;
            }
    }
    tTemp = tInt;
    return foundIntersection;
}

sf::Color Scene::raytracePixel(Ray newRay)
{
    double tInt = 1e+15;
    int objIndex = 0;
    bool foundIntersection = testRayAgainstScene(newRay, objIndex, tInt);

    sf::Color shadingColor = backgroundColor;

    if(foundIntersection)
    {
        Vec3d pi = newRay.o + newRay.d*tInt;

        MaterialProperties objectMaterial = objects[objIndex]->matProperties;

        if(newRay.order == 0)
            shadingColor = ambientLightingFactor*objectMaterial.diffuseColor;
        else
            shadingColor = sf::Color::Black;

        for(int i=0; i<(int)lights.size(); ++i)
        {
            Vec3d lightPos = lights[i]->getPos();
            Vec3d lightDir = Vec3d::norm(pi - lightPos);
            Ray lightRay(lightPos, lightDir, (pi-lightPos).dot(lightDir)-1e-6);

            if(!Shaders::isLightObstructed(lightRay, objects, objIndex))
            {
                shadingColor += Shaders::lambertShader(pi, objects[objIndex], lights[i]);
                shadingColor += Shaders::specular(pi, objects[objIndex], lights[i], newRay);
            }

            if(newRay.order < maxReflectionOrder && objectMaterial.reflectivity > 1e-15)
            {
                Vec3d objNormal = objects[objIndex]->getIntNormal(pi);
                Ray mirrorRay(pi + objNormal*1e-10, Shaders::calcReflectionVector(newRay.d,
                                                                objNormal),
                                                                  1e+15,
                                                                  newRay.order + 1);
                shadingColor += objectMaterial.reflectivity * raytracePixel(mirrorRay) ;
            }

        }
        clampColor(shadingColor);
    }

    return shadingColor;
}

void Scene::setAmbientFactor(double factor)
{
    ambientLightingFactor = factor;
}

void Scene::setBackgroundColor(sf::Color color)
{
    backgroundColor = color;
}

void Scene::setReflectionOrder(int order)
{
    maxReflectionOrder = order;
}
