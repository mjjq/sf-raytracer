# sf-raytracer
A 3D raytracer utilising SFML

## Summary:

A simple 3D raytracer which uses the SFML library as a window manager/drawing utility. Each rendering pixel fires 
a ray toward the scene, the colour of the pixel is then calculated based on the geometry of the scene with shading
applied to the geometry.

## Features:
- Support for multiple lights and objects with custom material parameters.

- Supports parametric geometry. New geometry can be easily defined by inheriting the base class and specifying algorithms
for ray intersection and normal calculation.

- Camera supports perspective projection with real time adjustable field of view. Camera uses quaternion based rotation
combined with real time movement for first person controls.

## Controls:

Standard W,A,S,D controls apply for translational movement, with R,F controlling up/downwards movement.

To look around, hold down LMB and drag the mouse.

To adjust field of view, hold down RMB and drag the mouse up/down.

## Extra

This is a casual side project which I'll be adding to in the future. More features will come in time.
