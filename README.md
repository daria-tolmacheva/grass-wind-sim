# Daria Tolmacheva Real-Time Vegetation Simulation

## Overview

For my assignment I will implement a real time simulation of grass swaying in the wind, largly based on  "A simulation on grass swaying with dynamic wind force" (2016, https://link.springer.com/article/10.1007/s00371-016-1263-7#Sec11).
I may later expand it to implement collisions with physical objects as well.


## Plan

The general plan of different steps I need to do (not nessecarily complete or in order but includes some stretch goals as well):
- implement grass plant
  - expand to inplement patches of grass
- implement simulation
  - implement wind dynamics simulation
  - compute grass dynamics based on the wind
  - implement 3 grass motions
    - swinging
    - bending
    - twisting
- render as three connected straight lines
  - add tesselation and geometry shaders
- add user controls
  - wind controls
  - grass controls
- load and save scenes from files (for the demo)

Stretch goals:
- implement LOD for larger scenes
  - further optimise for larger scenes
- implement collision with phisical objects

(Potentially look at https://dl.acm.org/doi/pdf/10.1145/2699276.2699283 for implementation of those extras if time permits.)

## Grass Representation

Each grass blade is represented by the 4 control points of a Bezier curve, and each grass plant consists of multiple grass blades starting in the same point.
The Blade class represents a single grass blade and hold information relevant to the Bezier curve which represents it as well as any other information relevant to the deformation calculation of it (e.i. orientation vectors of blade segments).

The Grass class needs to include a vector of Blade instances to hold the information about all of the grass in the scene(or part of the scene of LOD techniques would be implemented).
