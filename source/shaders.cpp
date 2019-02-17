#include "shaders.hpp"


sf::Color Shaders::lambertShader(Vec3d pi,
                       RenderObject* object,
                       LightSource* light)
{
    sf::Color tempColor = sf::Color::Black;
    double diffCoef = object->matProperties.diffuseCoef;

    if(diffCoef > 1e-15)
    {
        Vec3d surfaceNorm = object->getIntNormal(pi);
        double tempFactor = (Vec3d::norm(light->getPos() - pi).dot(surfaceNorm));

        if(tempFactor > 0.0)
        {
            tempColor = light->getColor();
            sf::Color objColor = object->matProperties.diffuseColor;
            tempColor.r *= diffCoef * tempFactor * objColor.r/255;
            tempColor.g *= diffCoef * tempFactor * objColor.g/255;
            tempColor.b *= diffCoef * tempFactor * objColor.b/255;
        }
    }

    return tempColor;
}

sf::Color Shaders::specular(Vec3d pi,
                            RenderObject* object,
                            LightSource* light,
                            Ray cameraRay)
{
    double specularCoef = object->matProperties.specularCoef;
    sf::Color tempColor = sf::Color::Black;

    if(specularCoef > 1e-15)
    {
        Vec3d l = Vec3d::norm(light->getPos() - pi);
        Vec3d v = Vec3d::norm(cameraRay.o - pi);
        Vec3d h = Vec3d::norm(l+v);

        double tempFactor = (object->getIntNormal(pi)).dot(h);

        if(tempFactor > 0.0)
        {
            tempFactor = pow(tempFactor, 25);
            tempColor = light->getColor();
            sf::Color objColor = object->matProperties.diffuseColor;

            tempColor.r *= specularCoef * tempFactor * objColor.r/255;
            tempColor.g *= specularCoef * tempFactor * objColor.g/255;
            tempColor.b *= specularCoef * tempFactor * objColor.b/255;
            //tempColor = specularCoef * tempFactor * tempColor;
        }
    }

    return tempColor;
}

Vec3d Shaders::calcReflectionVector(Vec3d incidentDir, Vec3d normalDir)
{
    Vec3d dParallel = normalDir * incidentDir.dot(normalDir);
    return incidentDir - dParallel * 2.0;
}

bool Shaders::isLightObstructed(Ray lightRay,
                                  std::vector<RenderObject*> &sceneObjects,
                                  int ignoreObject)
{
    for(int i=0; i<(int)sceneObjects.size(); ++i)
    {
        double t = 1e+15;
        if(sceneObjects[i]->intersects(lightRay, t))
        {
            if(t < lightRay.tMax && t>0.0 && i != ignoreObject)
                return true;
        }
    }

    return false;

}
