#include "renderer.hpp"
#include "shaders.hpp"
#include "materials.hpp"
#include <iostream>


Renderer::Renderer()
{
    initialise();
}

void Renderer::initialise()
{
    imageToRender.create(width, height, sf::Color::Black);
    texture.create(width, height);

    //lights.push_back(new LightSource(Vec3d(250.0, 0000.0, 200.0), Vec3d(0.0, 0.0, -1.0), sf::Color::Red));
    //scene.addLight(new LightSource(Vec3d(-50.0, 0.0, 1000.0), Vec3d(0.0, 0.0, 1.0), sf::Color::White));
    //scene.addLight(new LightSource(Vec3d(-300.0, 250.0, 90.0), Vec3d(0.0, 0.0, 1.0), 0.5*sf::Color::White));
    scene.addLight(new LightSource(Vec3d(300.0, -250.0, 600.0), Vec3d(0.0, 0.0, 1.0), 1.0*sf::Color::White));
    scene.addLight(new LightSource(Vec3d(0.0, 400.0, 0.0), Vec3d(0.0, 0.0, 1.0), sf::Color::White));
    //scene.addLight(new LightSource(Vec3d(0.0, 0.0, 200.0), Vec3d(0.0, 0.0, 1.0), 0.3*sf::Color::Blue));

    MaterialProperties props = Material::whiteChalk;
    props.diffuseColor = sf::Color::Yellow;

    scene.addObject(new RenderObject({0.0, 0.0, 0.0}, {100.0, 0.0, 0.0}, Material::whiteCeramic));
    scene.addObject(new RenderObject({80.0, 80.0, 200.0}, {120.0, 0.0, 0.0}, props));
    scene.addObject(new Plane({0.0, 0.0, -90.0}, {0.0, 0.0, 1.0}, Material::whiteChalk));

    scene.setAmbientFactor(0.1);
    scene.setReflectionOrder(1);

    camera = Camera(Vec3d(-400.0, 0.0, 100.0), Vec3d(1.0, 0.0, 0.0), width, height);
    camera.setPhysicalSize('X', (double)width/(double)height, 300);
    camera.setHorizFOV(90.0);
    setMouseSensitivity(0.2);
}

void Renderer::setMouseSensitivity(double value)
{
    mouseSensitivity = value;
}

void Renderer::castRaysToScene()
{
    for(int y=0; y<height; ++y)
        for(int x=0; x<width; ++x)
        {
            Ray newRay = camera.generatePixelRay(x,y);
            setPixelArrayColor(x, y, scene.raytracePixel(newRay));
        }
}

double Renderer::getDotProdFromRayBounce(Vec3d lightSourceDir,
                                   Vec3d surfaceNorm,
                                   Vec3d cameraRayDir)
{
    double dotProduct = surfaceNorm.dot(lightSourceDir);
    if(dotProduct > 0.0)
        return 0.0;

    Vec3d dParallel = surfaceNorm * (dotProduct);
    Vec3d dPerp = lightSourceDir - dParallel;

    Vec3d bouncedDir = dPerp - dParallel;

    return -cameraRayDir.dot(bouncedDir);
}

void Renderer::setPixelArrayColor(int pixelX, int pixelY, sf::Color color)
{
    int baseArrayPos = (pixelY*width + pixelX) * 4;
    pixels[baseArrayPos] = color.r;
    pixels[baseArrayPos+1] = color.g;
    pixels[baseArrayPos+2] = color.b;
    pixels[baseArrayPos+3] = color.a;
}

void Renderer::mouseEvents(sf::RenderWindow &_window)
{
    sf::Vector2i mouseTemp = sf::Mouse::getPosition(_window);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        int deltaX = mouseTemp.x - mousePosition.x;
        int deltaY = mouseTemp.y - mousePosition.y;

        //lights[0]->rotateLightSource(-(double)deltaX/360.0, (double)deltaY/360.0);
        Vec3d newPos = camera.getCameraPos();
        newPos.x -= (double)deltaX;
        newPos.y -= (double)deltaY;
        camera.sphericalRotation(deltaX*mouseSensitivity, deltaY*mouseSensitivity);
    }
    else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        int deltaY = mouseTemp.y - mousePosition.y;

        camera.setFocalDist(deltaY);
    }

    mousePosition = mouseTemp;
}

void Renderer::renderLoop()
{
    sf::Clock timer;
    while(window.isOpen())
    {
        timer.restart();

        window.clear();

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseMoved)
            {
                mouseEvents(window);
            }
            float movementSpeed = 4.0;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                movementSpeed = 10.0;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                camera.translateCamera(CameraMovementDir::FORWARD, movementSpeed);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                camera.translateCamera(CameraMovementDir::LEFT, movementSpeed);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                camera.translateCamera(CameraMovementDir::BACKWARD, movementSpeed);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                camera.translateCamera(CameraMovementDir::RIGHT, movementSpeed);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                camera.translateCamera(CameraMovementDir::UP, movementSpeed);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                camera.translateCamera(CameraMovementDir::DOWN, movementSpeed);
        }

        castRaysToScene();
        imageToRender.create(width, height, pixels);

        texture.loadFromImage(imageToRender);
        sprite.setTexture(texture, true);

        window.draw(sprite);
        window.display();

        float remainingTime = frameTimeInSec - timer.getElapsedTime().asSeconds();
        if(remainingTime >= 0.0f)
            sf::sleep(sf::seconds(remainingTime));
    }
}
