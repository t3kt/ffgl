//
//  VoronoiGenerator.cpp
//  Gradients
//
//  Created by tekt on 5/11/18.
//

#define _USE_MATH_DEFINES

#include "VoronoiGenerator.h"
#include <FFGLLib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "../../lib/ffgl/utilities/utilities.h"

static CFFGLPluginInfo PluginInfo
(
 VoronoiGenerator::CreateInstance,  // Create method
 "TEVG",                // Plugin unique ID
 "VoronoiGenerator",                // Plugin name
 1,                  // API major version number
 000,                // API minor version number
 1,                  // Plugin major version number
 000,                // Plugin minor version number
 FF_SOURCE,              // Plugin type
 "Voronoi generator plugin",    // Plugin description
 "by tekt@optexture.com" // About
);

VoronoiGenerator::VoronoiGenerator()
: PluginBase()
, _scaleX(1, 0, 4)
, _scaleY(1, 0, 4)
, _uniformScale(1, 0, 5)
, _translateX(0, -10, 10)
, _translateY(0, -10, 10)
, _rotate(0, -M_PI, M_PI)
, _fillEnabled(true, "enabled", "disabled")
, _isolinesEnabled(false, "enabled", "disabled")
, _borderEnabled(true, "enabled", "disabled")
, _borderOffsetX(0.01, 0, 0.1f)
, _borderOffsetY(0.03, 0, 0.1f)
, _borderColorR(0, 0, 1)
, _borderColorG(0, 0, 1)
, _borderColorB(0, 0, 1)
, _speed(5, 0, 50) {

  _debugDump = std::make_unique<ActionParameter>([&]() {
    debugDump();
  });

  SetMinInputs(0);
  SetMaxInputs(0);

  RegisterParam(_scaleX, FF_TYPE_XPOS, "Scale X");
  RegisterParam(_scaleY, FF_TYPE_YPOS, "Scale Y");
  RegisterParam(_uniformScale, FF_TYPE_STANDARD, "Uniform Scale");
  RegisterParam(_translateX, FF_TYPE_XPOS, "Translate X");
  RegisterParam(_translateY, FF_TYPE_YPOS, "Translate Y");
  RegisterParam(_rotate, FF_TYPE_STANDARD, "Rotate");
  RegisterParam(_fillEnabled, "Fill Enabled");
  RegisterParam(_isolinesEnabled, "Isolines Enabled");
  RegisterParam(_borderEnabled, "Border Enabled");
  RegisterParam(_borderOffsetX, FF_TYPE_XPOS, "Border Offset X");
  RegisterParam(_borderOffsetY, FF_TYPE_YPOS, "Border Offset Y");
  RegisterParam(_borderColorR, FF_TYPE_RED, "Border Red");
  RegisterParam(_borderColorG, FF_TYPE_GREEN, "Border Green");
  RegisterParam(_borderColorB, FF_TYPE_BLUE, "Border Blue");
  RegisterParam(_speed, FF_TYPE_STANDARD, "Speed");
  RegisterParam(*_debugDump, "Debug Dump");
}

FFResult VoronoiGenerator::InitGL(const FFGLViewportStruct *vp) {
  std::cout << "VoronoiGenerator:InitGL BEGIN" << std::endl;
  if (m_shader.Compile(voronoiGenVertexShader.c_str(),
                       voronoiGenFragmentShader.c_str()) != GL_TRUE) {
    std::cerr << "VoronoiGenerator: failed to compile shader" << std::endl;
    return FF_FAIL;
  }

  m_time = 0.0;
  m_lastTicks = getTicks() / 10000.0;

  m_resolutionX = vp->width;
  m_resolutionY = vp->height;

  m_shader.BindShader();

  m_scaleLocation = m_shader.FindUniform("scale");
  m_translateLocation = m_shader.FindUniform("translate");
  m_rotateLocation = m_shader.FindUniform("rotation");
  m_enableLocation = m_shader.FindUniform("enabled");
  m_borderColorLocation = m_shader.FindUniform("borderColor");
  m_borderOffsetLocation = m_shader.FindUniform("borderOffset");
  m_resolutionLocation = m_shader.FindUniform("resolution");
  m_timeLocation = m_shader.FindUniform("iGlobalTime");

  m_shader.UnbindShader();
  std::cout << "VoronoiGenerator:InitGL END" << std::endl;

  return FF_SUCCESS;
}

FFResult VoronoiGenerator::Resize(const FFGLViewportStruct *vp) {
  m_resolutionX = vp->width;
  m_resolutionY = vp->height;

  return FF_SUCCESS;
}

FFResult VoronoiGenerator::DeInitGL() {
  std::cout << "VoronoiGenerator:DeInitGL" << std::endl;
  m_shader.FreeGLResources();

  return FF_SUCCESS;
}

void VoronoiGenerator::updateTime() {
  double ticks = getTicks() / 10000.0;
  double delta = ticks - m_lastTicks;
  m_lastTicks = ticks;
  m_time += delta * _speed.GetValue();
}

FFResult VoronoiGenerator::ProcessOpenGL(ProcessOpenGLStruct *pGL) {
  //std::cout << "VoronoiGenerator:ProcessOpenGL BEGIN" << std::endl;
  updateTime();

  m_shader.BindShader();

  glUniform2f(m_scaleLocation,
              _scaleX.GetValue() * _uniformScale.GetValue(),
              _scaleY.GetValue() * _uniformScale.GetValue());
  glUniform2f(m_translateLocation,
              _translateX.GetValue(),
              _translateY.GetValue());
  glUniform1f(m_rotateLocation,
              _rotate.GetValue());
  glUniform3f(m_enableLocation,
              _borderEnabled.GetValue() ? 1.0f : 0.0f,
              _fillEnabled.GetValue() ? 1.0f : 0.0f,
              _isolinesEnabled.GetValue() ? 1.0f : 0.0f);
  glUniform2f(m_borderOffsetLocation,
              _borderOffsetX.GetValue(),
              _borderOffsetY.GetValue());
  glUniform3f(m_borderColorLocation,
              _borderColorR.GetValue(),
              _borderColorG.GetValue(),
              _borderColorB.GetValue());
  glUniform2f(m_resolutionLocation,
              static_cast<float>(m_resolutionX),
              static_cast<float>(m_resolutionY));
  glUniform1f(m_timeLocation,
              static_cast<float>(m_time));

  glBegin(GL_QUADS);

  glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
  glVertex2f(-1, -1);

  glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
  glVertex2f(-1, 1);

  glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
  glVertex2f(1, 1);

  glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
  glVertex2f(1, -1);

  glEnd();

  m_shader.UnbindShader();
  //std::cout << "VoronoiGenerator:ProcessOpenGL END" << std::endl;

  return FF_SUCCESS;
}

void VoronoiGenerator::debugDump() const {
  std::cout << "VoronoiGenerator:DebugDump" << std::endl;
  std::cout << "\n"
  << "VoronoiGenerator DUMP\n"
  << std::setprecision(4)
  << std::boolalpha
  << "scale: " << _scaleX << ", " << _scaleY << "\n"
  << "uniform scale: " << _uniformScale << "\n"
  << "translate: " << _translateX << ", " << _translateY << "\n"
  << "rotate: " << _rotate << "\n"
  << "speed: " << _speed << "\n"
  << "fill: " << _fillEnabled << " border: " << _borderEnabled << "\n"
  << "border offset: " << _borderOffsetX << ", " << _borderOffsetY << "\n"
  << "border color: " << _borderColorR << ", " << _borderColorG << ", " << _borderColorB << "\n"
  << "time: " << m_time << "\n"
  << "res: " << m_resolutionX << " x " << m_resolutionY << "\n"
  << std::endl;
}
