//
//  VoronoiGenerator.cpp
//  Gradients
//
//  Created by tekt on 5/11/18.
//

#include "VoronoiGenerator.h"
#include <FFGLLib.h>
#include <iostream>
#include <iomanip>
#include "../../lib/ffgl/utilities/utilities.h"

enum VoroGenParam {
  VGPARAM_SCALE_X,
  VGPARAM_SCALE_Y,
  VGPARAM_UNIFORM_SCALE,
  VGPARAM_TRANSLATE_X,
  VGPARAM_TRANSLATE_Y,
  VGPARAM_ROTATE,
  VGPARAM_SPEED,
  VGPARAM_FILL_ENABLED,
  VGPARAM_BORDER_ENABLED,
  VGPARAM_BORDER_OFFSET_X,
  VGPARAM_BORDER_OFFSET_Y,
  VGPARAM_BORDER_COLOR_R,
  VGPARAM_BORDER_COLOR_G,
  VGPARAM_BORDER_COLOR_B,
  VGPARAM_DUMP,
};

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
: CFreeFrameGLPlugin() {
  SetMinInputs(0);
  SetMaxInputs(0);

  SetParamInfo(VGPARAM_SCALE_X, "Scale X", FF_TYPE_XPOS, 1.0f);
  SetParamInfo(VGPARAM_SCALE_Y, "Scale Y", FF_TYPE_YPOS, 1.0f);
  m_scaleX = 1.0f;
  m_scaleY = 1.0f;

  SetParamInfo(VGPARAM_UNIFORM_SCALE, "Uniform Scale", FF_TYPE_BRIGHTNESS, 1.0f);
  m_uniformScale = 1.0f;

  SetParamInfo(VGPARAM_TRANSLATE_X, "Translate X", FF_TYPE_XPOS, 0.0f);
  SetParamInfo(VGPARAM_TRANSLATE_Y, "Translate Y", FF_TYPE_YPOS, 0.0f);
  m_translateX = 0.0f;
  m_translateY = 0.0f;

  SetParamInfo(VGPARAM_ROTATE, "Rotate", FF_TYPE_STANDARD, 0.0f);
  m_rotate = 0.0f;

  SetParamInfo(VGPARAM_SPEED, "Speed", FF_TYPE_STANDARD, 0.5f);
  m_speed = 1.0f;

  SetParamInfo(VGPARAM_FILL_ENABLED, "Fill Enabled", FF_TYPE_BOOLEAN, true);
  m_fillEnabled = true;

  SetParamInfo(VGPARAM_BORDER_ENABLED, "Border Enabled", FF_TYPE_BOOLEAN, false);
  m_borderEnabled = false;

  SetParamInfo(VGPARAM_BORDER_OFFSET_X, "Border Offset X", FF_TYPE_XPOS, 0.01f);
  SetParamInfo(VGPARAM_BORDER_OFFSET_Y, "Border Offset Y", FF_TYPE_YPOS, 0.03f);
  m_translateX = 0.01f;
  m_translateY = 0.03f;

  SetParamInfo(VGPARAM_BORDER_COLOR_R, "Border R", FF_TYPE_RED, 0.0f);
  SetParamInfo(VGPARAM_BORDER_COLOR_G, "Border G", FF_TYPE_GREEN, 0.0f);
  SetParamInfo(VGPARAM_BORDER_COLOR_B, "Border B", FF_TYPE_BLUE, 0.0f);
  m_borderColorR = 0.0f;
  m_borderColorG = 0.0f;
  m_borderColorB = 0.0f;

  SetParamInfo(VGPARAM_DUMP, "Debug Dump", FF_TYPE_EVENT, false);
}

FFResult VoronoiGenerator::InitGL(const FFGLViewportStruct *vp) {
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
  m_rotateLocation = m_shader.FindUniform("rotate");
  m_enableLocation = m_shader.FindUniform("enabled");
  m_borderColorLocation = m_shader.FindUniform("borderColor");
  m_resolutionLocation = m_shader.FindUniform("resolution");
  m_timeLocation = m_shader.FindUniform("iGlobalTime");

  m_shader.UnbindShader();

  return FF_SUCCESS;
}

FFResult VoronoiGenerator::Resize(const FFGLViewportStruct *vp) {
  m_resolutionX = vp->width;
  m_resolutionY = vp->height;

  return FF_SUCCESS;
}

FFResult VoronoiGenerator::DeInitGL() {
  m_shader.FreeGLResources();

  return FF_SUCCESS;
}

void VoronoiGenerator::updateTime() {
  double ticks = getTicks() / 10000.0;
  double delta = ticks - m_lastTicks;
  m_lastTicks = ticks;
  m_time += delta * m_speed;
}

FFResult VoronoiGenerator::ProcessOpenGL(ProcessOpenGLStruct *pGL) {
//  auto t = m_time;
//  double ticks = getTicks() / 1000.0;
//  double lastFrameTime = ticks = m_lastTicks;
//  m_time += lastFrameTime;
//  std::cout << "time was " << t << " ticks: " << ticks << " lastticks: " << m_lastTicks << " delta: " << lastFrameTime << " new time: " << m_time << std::endl;
  updateTime();

  m_shader.BindShader();

  glUniform2f(m_scaleLocation,
              m_scaleX * m_uniformScale,
              m_scaleY * m_uniformScale);
  glUniform2f(m_translateLocation,
              m_translateX,
              m_translateY);
  glUniform1f(m_rotateLocation,
              m_rotate);
  glUniform2f(m_enableLocation,
              m_borderEnabled ? 1.0f : 0.0f,
              m_fillEnabled ? 1.0f : 0.0f);
  glUniform3f(m_borderColorLocation,
              m_borderColorR,
              m_borderColorG,
              m_borderColorB);
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

  return FF_SUCCESS;
}

float VoronoiGenerator::GetFloatParameter(unsigned int index) {
  switch (index) {
    case VGPARAM_SCALE_X:
      return m_scaleX;
    case VGPARAM_SCALE_Y:
      return m_scaleY;
    case VGPARAM_UNIFORM_SCALE:
      return m_uniformScale;
    case VGPARAM_TRANSLATE_X:
      return m_translateX;
    case VGPARAM_TRANSLATE_Y:
      return m_translateY;
    case VGPARAM_ROTATE:
      return m_rotate;
    case VGPARAM_SPEED:
      return m_speed / 2.0f;
    case VGPARAM_FILL_ENABLED:
      return m_fillEnabled ? 1.0f : 0.0f;
    case VGPARAM_BORDER_ENABLED:
      return m_borderEnabled ? 1.0f : 0.0f;
    case VGPARAM_BORDER_OFFSET_X:
      return m_borderOffsetX;
    case VGPARAM_BORDER_OFFSET_Y:
      return m_borderOffsetY;
    case VGPARAM_BORDER_COLOR_R:
      return m_borderColorR;
    case VGPARAM_BORDER_COLOR_G:
      return m_borderColorG;
    case VGPARAM_BORDER_COLOR_B:
      return m_borderColorB;
    default:
      return 0.0f;
  }
}

FFResult VoronoiGenerator::SetFloatParameter(unsigned int dwIndex,
                                             float value) {
  switch (dwIndex) {
    case VGPARAM_SCALE_X:
      m_scaleX = value;
      break;
    case VGPARAM_SCALE_Y:
      m_scaleY = value;
      break;
    case VGPARAM_UNIFORM_SCALE:
      m_uniformScale = value;
      break;
    case VGPARAM_TRANSLATE_X:
      m_translateX = value;
      break;
    case VGPARAM_TRANSLATE_Y:
      m_translateY = value;
      break;
    case VGPARAM_ROTATE:
      m_rotate = value;
      break;
    case VGPARAM_SPEED:
      m_speed = value / 0.5f;
      break;
    case VGPARAM_FILL_ENABLED:
      m_fillEnabled = value >= 0.5f;
      break;
    case VGPARAM_BORDER_ENABLED:
      m_borderEnabled = value >= 0.5f;
      break;
    case VGPARAM_BORDER_OFFSET_X:
      m_borderOffsetX = value;
      break;
    case VGPARAM_BORDER_OFFSET_Y:
      m_borderOffsetY = value;
      break;
    case VGPARAM_BORDER_COLOR_R:
      m_borderColorR = value;
      break;
    case VGPARAM_BORDER_COLOR_G:
      m_borderColorG = value;
      break;
    case VGPARAM_BORDER_COLOR_B:
      m_borderColorB = value;
      break;
    case VGPARAM_DUMP:
      if (value >= 0.5) {
        debugDump();
      }
      break;
    default:
      return FF_FAIL;
  }
  return FF_SUCCESS;
}

void VoronoiGenerator::debugDump() const {
  std::cout << "\n"
  << "VoronoiGenerator DUMP\n"
  << std::setprecision(4)
  << std::boolalpha
  << "scale: " << m_scaleX << ", " << m_scaleY << "\n"
  << "uniform scale: " << m_uniformScale << "\n"
  << "translate: " << m_translateX << ", " << m_translateY << "\n"
  << "rotate: " << m_rotate << "\n"
  << "speed: " << m_speed << "\n"
  << "fill: " << m_fillEnabled << " border: " << m_borderEnabled << "\n"
  << "border offset: " << m_borderOffsetX << ", " << m_borderOffsetY << "\n"
  << "border color: " << m_borderColorR << ", " << m_borderColorG << ", " << m_borderColorB << "\n"
  << "time: " << m_time << "\n"
  << "res: " << m_resolutionX << " x " << m_resolutionY << "\n"
  << std::endl;
}
