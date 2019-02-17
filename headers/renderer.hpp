#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "camera.hpp"
#include "scene.hpp"
#include <vector>

class Renderer
{
    int width = 800;
    int height = 600;
    sf::RenderWindow window{sf::VideoMode(width, height), "RayTracer"};
    float targetFramerate = 16.0f;
    float frameTimeInSec = 1.0f/targetFramerate;

    sf::Vector2i mousePosition;
    double mouseSensitivity = 1.0;
    void mouseEvents(sf::RenderWindow &_window);
    void setMouseSensitivity(double value);

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image imageToRender;//(width, height, sf::Color(0,0,0,0));
    sf::Uint8 *pixels = new sf::Uint8[width*height*4];

    Scene scene;
    Camera camera = Camera(Vec3d(-200.0, 0.0, 100.0),
                           Vec3d(1.0, 0.0, -0.0),
                           width, height);

    void castRaysToScene();
    void initialise();
    void setPixelArrayColor(int pixelX, int pixelY, sf::Color color);
    double getDotProdFromRayBounce(Vec3d lightSourceDir,
                                   Vec3d surfaceNorm,
                                   Vec3d cameraRayDir);
public:
    Renderer();
    void renderLoop();
};


#endif // RENDERER_HPP
