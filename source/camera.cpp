#include "camera.hpp"

#include <cmath>
#include <iostream>

Camera::Camera(Vec3d pos, Vec3d dir, int pixelX, int pixelY) :
    cameraPos{pos}, pixelsX{pixelX}, pixelsY{pixelY}
{
    setCameraDir(dir);
    physicalSizeX = (double)pixelsX;
    physicalSizeY = (double)pixelsY;
}

void Camera::setCameraPos(Vec3d pos)
{
    cameraPos = pos;
}

void Camera::setCameraDir(Vec3d dir)
{
    cameraAxes = ONormalSet(dir);

    updateFocalPoint();
}

void Camera::updateFocalPoint()
{
    focalPointPos = getCameraDir()*-1.0*focalPointDist + cameraPos;
}

Vec3d Camera::getCameraPos()
{
    return cameraPos;
}

Vec3d Camera::getCameraDir()
{
    return cameraAxes.kRot;
}

Vec3d Camera::getCameraDir(Vec3d pixelPos)
{
    return Vec3d::norm(pixelPos - focalPointPos);
}

Vec3d Camera::getPlaneX()
{
    return cameraAxes.iRot;
}

Vec3d Camera::getPlaneY()
{
    return cameraAxes.jRot;
}

void Camera::orbitCamera(double deltaPhi,
                         double deltaTheta)
{
    double cosPhi = cos(deltaPhi);
    double sinPhi = sin(deltaPhi);

    std::cout << cameraPos.x << "\n";

    double x = cameraPos.x * cosPhi - cameraPos.y * sinPhi;
    double y = cameraPos.x * sinPhi + cameraPos.y * cosPhi;
    double z = cameraPos.z;

    Vec3d newPos(x,y,z);

    cameraPos = newPos;
    setCameraDir(Vec3d::norm(newPos)*-1.0);

    std::cout << cameraPos.x << "\n\n";
}

void Camera::sphericalRotation(double deltaPhi,
                               double deltaTheta)
{
   /*phi += deltaPhi;
   theta += deltaTheta;

    double cosPhi = cos(phi);
    double sinPhi = sin(phi);
    double sinTheta = sin(theta);
    double cosTheta = cos(theta);

    Vec3d newSins;
    newSins.x = sinTheta * cosPhi;
    newSins.y = sinTheta * sinPhi;
    newSins.z = cosTheta;

    //cameraPos = newSins * targetDistance;
    setCameraDir(newSins);*/
    cameraAxes.relRotate(deltaPhi, cameraAxes.k);
    cameraAxes.relRotate(deltaTheta, cameraAxes.iRot);
    updateFocalPoint();
}

void Camera::translateCamera(CameraMovementDir dir, double speed)
{
    switch(dir)
    {
        case(CameraMovementDir::FORWARD):
            cameraPos = cameraPos + getCameraDir() * speed;
            break;
        case(CameraMovementDir::BACKWARD):
            cameraPos = cameraPos - getCameraDir() * speed;
            break;
        case(CameraMovementDir::LEFT):
            cameraPos = cameraPos - getPlaneX() * speed;
            break;
        case(CameraMovementDir::RIGHT):
            cameraPos = cameraPos + getPlaneX() * speed;
            break;
        case(CameraMovementDir::DOWN):
            cameraPos = cameraPos + getPlaneY() * speed;
            break;
        case(CameraMovementDir::UP):
            cameraPos = cameraPos - getPlaneY() * speed;
            break;

        default:
            break;
    }
    updateFocalPoint();
}

void Camera::setFocalDist(double dist)
{
    focalPointDist = focalPointDist + dist;
    updateFocalPoint();
}

void Camera::setPhysicalSize(double physX, double physY)
{
    physicalSizeX = physX;
    physicalSizeY = physY;
}

void Camera::setPhysicalSize(char dimension, double aspectRatio, double length)
{
    switch(dimension)
    {
        case 'X':
            physicalSizeX = length;
            physicalSizeY = length / aspectRatio;
            break;
        case 'Y':
            physicalSizeY = length;
            physicalSizeX = length*aspectRatio;
            break;
        default:
            break;
    }
}

void Camera::setHorizFOV(double fovInDegrees)
{
    double halfAngle = 0.5 * fovInDegrees * Constants::PI / 180.0;

    focalPointDist = physicalSizeX / tan(halfAngle);
    updateFocalPoint();



}

Ray Camera::generatePixelRay(int x, int y)
{
    Vec3d startPos = getPlaneX()*((double)x/(double)pixelsX - 0.5)*physicalSizeX +
                     getPlaneY()*((double)y/(double)pixelsY - 0.5)*physicalSizeY;
    startPos = startPos + getCameraPos();
    Vec3d dir = getCameraDir(startPos);

    return Ray(startPos, dir);
}
