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

NGLScene::NGLScene() : m_grass(3), m_simulation(10.0, 60, 20, 60, 0.2)
{
    m_simulation.setVelocity(2.0f, X_POS);
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

//    ngl::Vec3 from(-5.0f, 15.0f, -5.0f); // edited by me
//    ngl::Vec3 to(20.0f, 5.0f, 20.0f);
//    ngl::Vec3 up(0.0f, 1.0f, 0.0f);

    m_view = ngl::lookAt(from, to, up);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10

    // Edited by me
    // grab an instance of shader manager
//    ngl::ShaderLib::loadShader("CurveShader", "shaders/grassVertex.glsl",
//                               "shaders/grassFragment.glsl",
//                               "shaders/grassGeometry.glsl");
//    ngl::ShaderLib::use("CurveShader");

    ngl::ShaderLib::createShaderProgram("GrassShader");

    // now we are going to create empty shaders for Frag and Vert
    ngl::ShaderLib::attachShader("GrassVertex",   ngl::ShaderType::VERTEX);
//    ngl::ShaderLib::attachShader("GrassControl",  ngl::ShaderType::TESSCONTROL);
//    ngl::ShaderLib::attachShader("GrassEval",     ngl::ShaderType::TESSEVAL);
    ngl::ShaderLib::attachShader("GrassGeom",     ngl::ShaderType::GEOMETRY);
    ngl::ShaderLib::attachShader("GrassFragment", ngl::ShaderType::FRAGMENT);

    // attach the source
    ngl::ShaderLib::loadShaderSource("GrassVertex",   "shaders/grassVertex.glsl");
//    ngl::ShaderLib::loadShaderSource("GrassControl",  "shaders/grassTessControl.glsl");
//    ngl::ShaderLib::loadShaderSource("GrassEval",     "shaders/grassTessEval.glsl");
    ngl::ShaderLib::loadShaderSource("GrassGeom",     "shaders/grassGeometry.glsl");
    ngl::ShaderLib::loadShaderSource("GrassFragment", "shaders/grassFragment.glsl");

    // add them to the program
    ngl::ShaderLib::attachShaderToProgram("GrassShader", "GrassVertex");
//    ngl::ShaderLib::attachShaderToProgram("GrassShader", "GrassControl");
//    ngl::ShaderLib::attachShaderToProgram("GrassShader", "GrassEval");
    ngl::ShaderLib::attachShaderToProgram("GrassShader", "GrassGeom");
    ngl::ShaderLib::attachShaderToProgram("GrassShader", "GrassFragment");

    // compile the shaders
    ngl::ShaderLib::compileShader("GrassVertex");
//    ngl::ShaderLib::compileShader("GrassControl");
//    ngl::ShaderLib::compileShader("GrassEval");
    ngl::ShaderLib::compileShader("GrassGeom");
    ngl::ShaderLib::compileShader("GrassFragment");

    // now we have associated this data we can link the shader
    ngl::ShaderLib::linkProgramObject("GrassShader");

    ngl::ShaderLib::use("GrassShader");
    ngl::ShaderLib::autoRegisterUniforms("GrassShader");
    ngl::ShaderLib::printRegisteredUniforms("GrassShader");
    ngl::ShaderLib::setUniform("steps", 0.01f);
    GLuint id = ngl::ShaderLib::getProgramID("GrassShader");
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
                                                  {0.0f,   1.5f,   -11.5f} };

    //plantControlPoints =
    m_grass.setBlades(plantControlPoints);
    m_grass.createVAO();
    glEnable(GL_DEPTH_TEST);

    ngl::VAOPrimitives::createLineGrid("floor", 40, 40, 10);

    startTimer(5);
    m_previousTime = std::chrono::steady_clock::now();
// end of edit
}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib::use("GrassShader");
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

void NGLScene::timerEvent(QTimerEvent *event)
{
    auto now = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration<float, std::chrono::seconds::period>(now - m_previousTime).count();
    m_previousTime = now;
    // update sim
    m_simulation.simulate(delta, 40);
    // update grass
    m_grass.updateBlades(m_simulation, delta);
    update();
}