# Weekly Plan for Development

## Week 1

1. Flesh out the plan to understand the work that needs to be done
2. Basic project setup (CMakeLists.txt, directories structure etc)
3. Figure out sample grass plant control points to hard-code initially.
4. Initial implementation of Grass class and tests for it.

## Week 2

Didn't have much time this week
1. Research math for the wind simulation and grass blade deformation

## Week 3

1. Fixes from feedback + wrap up basics from before.
2. Class diagram and Design.md
3. Refactor code to use `ngl::Vec3` instead of `Point`.
4. Implement and test orientation vectors of grass blade segments in `Blade`.

## Week 4 

BFX week

## Week 5

1. Add grass blade attributes description to `README.md`.
2. Basic OpenGL visualisation - just to add the skeleton of the visualisation code in. This will later be edited to use shaders to draw grass geometry directly from control points (and oriantation vectors) without using NGL bezier curves. 
3. Basic Qt app to host OpenGL visualisation. Will change mouse/keyboard controls and add more control to the user.

## Week 6

1. Fix camera controls a bit.
2. Update the visualisation to include custom shaders rather then use ngl::Curve.
3. Implement tessellation/geometry shaders(?)

## Week 7

TBD
1. Implement tessellation/geometry shaders(?)
2. Refactor `Grass`/`Blade` to better fit shader needs(?)
3. Add WindActor class + tests.

## Week 8 (during Christmas)

TBD
1. Wind simulation on CPU.
2. Refactor the fluid simulation to be calculated on GPU.
3. Clean up documentation: update `README.md` and `Design.md`

## Week 9

TBD

1. Simulation user controls.
2. Update camera controls to fit my needs.
3. Reading/writing files for demos.

## Week 10

TBD
1. Design demos.
2. Cleaning up.
3. Finishing up anything that still needs to be done.