# Daria Tolmacheva Real-Time Vegetation Simulation

## Overview

For my assignment I will implement a real time simulation of grass swaying in the wind, largely based on  "A simulation on grass swaying with dynamic wind force" (2016, https://link.springer.com/article/10.1007/s00371-016-1263-7).
I may later expand it to implement collisions with physical objects as well.

## Running instructions
This project depends on `NCCA NGL Library`, so please first make sure that all of the dependancies for it are resolved and it works in your environment.

Before running make sure that the `GrassSimulation` build is configured to run `GrassSimulationCopyShaders` target before launch.

Unfortunately there are currently no controls implemented. In order to change the values you need to modify the variables in code. Here is the list of things that can be changed and where(you can follow the link to GitHub by clicking the code snippet):
- Air density, simulation grid dimensions and single cell size - [`NGLScene.cpp#L14 - m_simulation(_density, _x, _y, _z, _cellSize)`](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/src/NGLScene.cpp#L14)
- Wind velocity and direction (X_POS, Z_POS, X_NEG or Z_NEG) - [`NGLScene.cpp#L16 - m_simulation.setVelocity(_velocity, _direction);`](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/src/NGLScene.cpp#L16)
- Fluid dynamics over relaxation (should be between 1 and 2) - [`Simulation.h#L47 - overRelaxation = _coef`](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/include/Simulation.h#L47)
- Grass dencity - [`Blade.cpp#L10 - m_dencity = _dencity`](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/src/Blade.cpp#L10)
- Wind force coefficient - [`Blade.cpp#L9 - m_windCoof = _coef`](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/src/Blade.cpp#L9)
- Damping force coefficient - [`Blade.cpp#L11 - m_dampingCoof = _coef`](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/src/Blade.cpp#L11)
- Segment stiffness coefficient - [`Blade.h#L39 - m_stiffness = {_bottomSeg, _midSeg, _topSeg}`](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/include/Blade.h#L39)


The current hard coded values make the frame rate quite slow and the quality of the animation jagged, however this is the best set of parameters in terms of resulting grass motion form out of the limited set of experiments I've tried, so for purposes of the default I have settled for them for now despite lower frame rate.

## Speed Concerns

Currently the project needs to be refactored to optimise the performance. While this may include computing the fluid simulation on GPU via a compute shader, I should refactor the way OpenGL buffers are handled to perhaps store control points directly in a vector in `Grass`. Similarly, I need to go over the code and make sure to pass parameters by reference where needed to avoid copying large vectors.

## Notes

Please refer to [`Design.md` file](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/Design.md) for mode details of the design decisions and explanations of implementation, as well as [`Plan.md` file](https://github.com/NCCA/ase-assignment-daria-tolmacheva/blob/main/Plan.md) for my progress log throughout the project duration.

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
