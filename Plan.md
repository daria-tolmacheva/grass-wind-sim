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

1. Implement tessellation/geometry shaders(?)

## After Christmas

1. Implement wind simulation on CPU.
2. Implement grass dynamics.
3. Connect everything into the simulation loop.
4. Cleaning up.
5. Updating documentation (README.md and Design.md)

## Original Plan

The general plan of different steps I need to do (not necessarily complete or in order but includes some stretch goals as well) - completed tasks are crossed out:
- ~implement grass plant~
  - expand to inplement patches of grass
- ~implement simulation~
  - ~implement wind dynamics simulation~
  - ~compute grass dynamics based on the wind~
  - implement 3 grass motions
    - ~swinging~
    - ~bending~
    - twisting
- ~render as three connected straight lines~
  - add tesselation and ~geometry shaders~
- add user controls
  - wind controls
  - grass controls
- load and save scenes from files (for the demo)

Extra potential stretch goals:
- implement LOD for larger scenes
  - further optimise for larger scenes
- implement collision with physical objects

## Further Improvements

As you can see above the main further improvements that are currently needed are:
1. Implement a GUI or keyboard controls to change simulation settings.
2. Third grass motion implementation.
3. Tessellation and geometry shader generating a triangle strip result.
4. Performance improvements including GPU fluid dynamics.