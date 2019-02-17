#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include "renderObject.hpp"

enum class CameraMovementDir
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
    Vec3d cameraPos;

    ONormalSet cameraAxes;
    //define cameraAxes.k as the camera plane normal

    Vec3d focalPointPos;
    double focalPointDist = 500.0;

    double physicalSizeX;
    double physicalSizeY;

    int pixelsX;
    int pixelsY;

    double targetDistance = 200.0;

public:
    Camera(Vec3d pos, Vec3d dir, int pixelX, int pixelY);

    void setCameraPos(Vec3d pos);
    void setCameraDir(Vec3d dir);
    void updateFocalPoint();

    Vec3d getCameraPos();
    Vec3d getCameraDir();
    Vec3d getCameraDir(Vec3d pixelPos);

    Vec3d getPlaneX();
    Vec3d getPlaneY();

    void orbitCamera(double deltaPhi,
                    double deltaTheta);

    void sphericalRotation(double deltaPhi,
               double deltaTheta);

    void translateCamera(CameraMovementDir dir, double speed);

    void setFocalDist(double dist);
    void setPhysicalSize(double physX, double physY);
    void setPhysicalSize(char dimension, double aspectRatio, double length);
    void setHorizFOV(double fovInDegrees);

    Ray generatePixelRay(int x, int y);
};

#endif // CAMERA_HPP
