/// This file is edited from the NGL GeometryShaders Demo by Jon Macey:
/// https://github.com/NCCA/GeometryShaders/blob/main/Curves/shaders/curveFragment.glsl
/// commit 6e2b48b

#version 410 core

/// @brief our output fragment colour
layout (location = 0) out vec4 fragColour;

void main ()
{
    fragColour =  vec4(0.65, 0.80, 0.55, 1);
}

