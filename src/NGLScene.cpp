#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <iostream>

#include "Grass.h"

/// This file is edited from the NGL Curve Demo file by Jon Macey:
/// https://github.com/NCCA/CurveDemos/blob/main/CurveDemo/src/NGLScene.cpp
/// commit 20351cf

NGLScene::NGLScene()
{
  setTitle("Using ngl::BezierCurve");
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
  ngl::Vec3 from(-25.0f, 15.0f, -25.0f);
  ngl::Vec3 to(0.0f, 5.0f, 0.0f);
  ngl::Vec3 up(0.0f, 1.0f, 0.0f);
  m_view = ngl::lookAt(from, to, up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_project = ngl::perspective(45.0f, 720.0f / 576.0f, 0.5f, 150.0f);

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);


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

  m_curves.resize(plantControlPoints.size() * 0.25);
  for (size_t i = 0; i < m_curves.size(); ++i)
  {
    m_curves[i] = std::make_unique<ngl::BezierCurve>();
    m_curves[i]->addPoint(plantControlPoints[i*4]);
    m_curves[i]->addPoint(plantControlPoints[i*4 + 1]);
    m_curves[i]->addPoint(plantControlPoints[i*4 + 2]);
    m_curves[i]->addPoint(plantControlPoints[i*4 + 3]);
    m_curves[i]->setLOD(200.0f);
    m_curves[i]->createKnots();
    m_curves[i]->createVAO();
  }
}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::Mat4 MVP;
  MVP = m_project * m_view * m_mouseGlobalTX;
  ngl::ShaderLib::setUniform("MVP", MVP);
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
  m_mouseGlobalTX = rotY * rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
  loadMatricesToShader();

  ngl::ShaderLib::use("nglColourShader");
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);
  for (auto & curve : m_curves)
  {
    curve->draw();
  }
  glPointSize(4);
  ngl::ShaderLib::setUniform("Colour", 0.0f, 1.0f, 0.0f, 1.0f);

  for (auto & curve : m_curves)
  {
    curve->drawControlPoints();
  }
  glPointSize(1);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 0.0f, 0.0f, 1.0f);

  for (auto & curve : m_curves)
  {
    curve->drawHull();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent(QMouseEvent *_event)
{
// note the method buttons() is the button state when event was called
// that is different from button() which is used to check which button was
// pressed when the mousePress/Release event is generated
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif

  if (m_win.rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx = position.x() - m_win.origX;
    int diffy = position.y() - m_win.origY;
    m_win.spinXFace += static_cast<int>(0.5f * diffy);
    m_win.spinYFace += static_cast<int>(0.5f * diffx);
    m_win.origX = position.x();
    m_win.origY = position.y();
    update();
  }
  // right mouse translate code
  else if (m_win.translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = static_cast<int>(position.x() - m_win.origXPos);
    int diffY = static_cast<int>(position.y() - m_win.origYPos);
    m_win.origXPos = position.x();
    m_win.origYPos = position.y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent(QMouseEvent *_event)
{
// that method is called when the mouse button is pressed in this case we
// store the value where the maouse was clicked (x,y) and set the Rotate flag to true
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif

  if (_event->button() == Qt::LeftButton)
  {
    m_win.origX = position.x();
    m_win.origY = position.y();
    m_win.rotate = true;
  }
  // right mouse translate mode
  else if (_event->button() == Qt::RightButton)
  {
    m_win.origXPos = position.x();
    m_win.origYPos = position.y();
    m_win.translate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent(QMouseEvent *_event)
{
  // that event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_win.rotate = false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_win.translate = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

  // check the diff of the wheel position (0 means no change)
  if (_event->angleDelta().x() > 0)
  {
    m_modelPos.m_z += ZOOM;
  }
  else if (_event->angleDelta().x() < 0)
  {
    m_modelPos.m_z -= ZOOM;
  }
  update();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape:
    QGuiApplication::exit(EXIT_SUCCESS);
    break;
  // turn on wirframe rendering
  case Qt::Key_W:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  // turn off wire frame
  case Qt::Key_S:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  // show full screen
  case Qt::Key_F:
    showFullScreen();
    break;
  // show windowed
  case Qt::Key_N:
    showNormal();
    break;
  default:
    break;
  }
  update();
}
