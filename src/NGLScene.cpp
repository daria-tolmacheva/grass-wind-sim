#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <iostream>

/// This file is edited from the NGL Curve Demo file by Jon Macey:
/// https://github.com/NCCA/CurveDemos/blob/main/CurveDemo/src/NGLScene.cpp
/// commit 20351cf

NGLScene::NGLScene() : m_grass(4)
{
  setTitle("Grass Simulation");
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_project = ngl::perspective(45.0f, static_cast<float>(_w) / _h, 0.05f, 350.0f);
  m_win.width = static_cast<int>(_w * devicePixelRatio());
  m_win.height = static_cast<int>(_h * devicePixelRatio());
}

/// This function is edited by me
void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::initialize();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(-25.0f, 15.0f, -25.0f); // edited by me
  ngl::Vec3 to(0.0f, 5.0f, 0.0f);
  ngl::Vec3 up(0.0f, 1.0f, 0.0f);
  m_view = ngl::lookAt(from, to, up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10

  // Edited by me
  // grab an instance of shader manager
  ngl::ShaderLib::loadShader("CurveShader", "shaders/grassVertex.glsl",
                             "shaders/grassFragment.glsl",
                             "shaders/grassGeometry.glsl");
  ngl::ShaderLib::use("CurveShader");
  ngl::ShaderLib::setUniform("steps", 0.01f);
  GLuint id = ngl::ShaderLib::getProgramID("CurveShader");
  m_subroutines[0] = glGetSubroutineIndex(id, GL_GEOMETRY_SHADER, "bezier");
  m_subroutines[1] = glGetSubroutineIndex(id, GL_GEOMETRY_SHADER, "lerpCurve");

  std::vector<ngl::Vec3> plantControlPoints = { {0.0f,   0.0f,   0.0f}, // first blade
                                                {0.0f,   7.0f,   1.0f},
                                                {0.0f,   9.0f,   4.0f},
                                                {0.0f,   10.0f,  6.0f},
                                                {0.0f,   0.0f,   0.0f}, // second blade
                                                {0.0f,   7.0f,   0.0f},
                                                {0.5f,   10.0f,  0.0f},
                                                {3.0f,   14.0f,  0.0f},
                                                {0.0f,   0.0f,   0.0f}, // third blade
                                                {0.0f,   4.0f,   -0.5f},
                                                {0.0f,   4.5f,   -5.0f},
                                                {0.0f,   1.5f,   -11.5f},
                                                {0.0f,   0.0f,   0.0f}, // forth blade
                                                {-1.0f,  8.0f,   0.0f},
                                                {-12.5f, 3.5f,   0.0f},
                                                {-16.0f, 1.5f,   0.0f} };

  m_grass.setBlades(plantControlPoints);
  m_grass.createVAO();
  glEnable(GL_DEPTH_TEST);

  ngl::VAOPrimitives::createLineGrid("floor", 40, 40, 10);
// end of edit
}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib::use("CurveShader");
  ngl::ShaderLib::setUniform("MVP", m_project * m_view * m_mouseGlobalTX);
  glUniformSubroutinesuiv(GL_GEOMETRY_SHADER, 1, &m_subroutines[m_activeSubroutine]);
  ngl::ShaderLib::setUniform("steps", m_steps);
}
void NGLScene::loadMatricesToColourShader(const ngl::Vec4 &_colour) {
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", _colour);
  ngl::ShaderLib::setUniform("MVP", m_project * m_view * m_mouseGlobalTX);
}

/// This function is edited by me
void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_win.width, m_win.height);
  // Rotation based on the mouse position for our global transform
  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX = rotX * rotY;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  // Edited by me
  // Draw grass
  loadMatricesToShader();
  m_grass.draw();

  // Draw ground grid
  // loadMatricesToColourShader(ngl::Vec4(0.6f, 0.29f, 0.0f, 1.0f)); <- doesn't pass the colour for some reason(?)
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 0.6f, 0.29f, 0.0f, 1.0f);
  ngl::ShaderLib::setUniform("MVP", m_project * m_view * m_mouseGlobalTX);
  ngl::VAOPrimitives::draw("floor");
  // end of edit
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
    // escape key to quite
    case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
    case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

      break;
    default : break;
  }
  // finally update the GLWindow and re-draw
  update();
}