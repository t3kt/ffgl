//
//  VoronoiGenerator.h
//  Gradients
//
//  Created by tekt on 5/11/18.
//

#pragma once

#include <FFGLPluginSDK.h>
#include <FFGLShader.h>
#include <string>
#include "../../shared/Plugin.h"
#include <memory>

extern std::string voronoiGenVertexShader;
extern std::string voronoiGenFragmentShader;

class VoronoiGenerator : public PluginBase {
public:
  VoronoiGenerator();
  virtual ~VoronoiGenerator() {}

  ///////////////////////////////////////////////////
  // FreeFrame plugin methods
  ///////////////////////////////////////////////////

  FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
  FFResult InitGL(const FFGLViewportStruct *vp) override;
  FFResult DeInitGL() override;

  unsigned int Resize(const FFGLViewportStruct *vp) override;

  //The mixer short name
  const char * GetShortName() override { static const char* sname = "VoronoiGen"; return sname; }

  ///////////////////////////////////////////////////
  // Factory method
  ///////////////////////////////////////////////////

  static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
  {
    *ppOutInstance = new VoronoiGenerator();
    if (*ppOutInstance != NULL)
      return FF_SUCCESS;
    return FF_FAIL;
  }

private:
  void updateTime();
  void debugDump() const;

  FloatParameter _scaleX;
  FloatParameter _scaleY;
  FloatParameter _uniformScale;
  FloatParameter _translateX;
  FloatParameter _translateY;
  FloatParameter _rotate;
  BoolParameter _fillEnabled;
  BoolParameter _borderEnabled;
  BoolParameter _isolinesEnabled;
  FloatParameter _borderOffsetX;
  FloatParameter _borderOffsetY;
  FloatParameter _borderColorR;
  FloatParameter _borderColorG;
  FloatParameter _borderColorB;
  FloatParameter _speed;
  std::shared_ptr<ActionParameter> _debugDump;
  double m_lastTicks;
  double m_time;

  GLuint m_resolutionX;
  GLuint m_resolutionY;

  FFGLShader m_shader;

  GLint m_scaleLocation;
  GLint m_translateLocation;
  GLint m_rotateLocation;
  GLint m_enableLocation;
  GLint m_borderOffsetLocation;
  GLint m_borderColorLocation;
  GLint m_resolutionLocation;
  GLint m_timeLocation;
};
