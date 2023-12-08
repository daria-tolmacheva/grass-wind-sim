/// This file is edited from the NGL GeometryShaders Demo by Jon Macey:
/// https://github.com/NCCA/GeometryShaders/blob/main/Curves/shaders/curveVertex.glsl
/// commit 1a8ac3a

#version 410 core
/// @brief the vertex passed in
layout (location = 0) in vec3 inVert;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP*vec4(inVert,1);
}
