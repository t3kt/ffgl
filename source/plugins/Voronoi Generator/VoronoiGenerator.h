//
//  VoronoiGenerator.hpp
//  Gradients
//
//  Created by tekt on 5/11/18.
//

#pragma once

#include <FFGLPluginSDK.h>
#include <FFGLShader.h>
#include <string>

extern std::string voronoiGenVertexShader;
extern std::string voronoiGenFragmentShader;

class VoronoiGenerator : public CFreeFrameGLPlugin {
public:
  VoronoiGenerator();
  virtual ~VoronoiGenerator() {}

  ///////////////////////////////////////////////////
  // FreeFrame plugin methods
  ///////////////////////////////////////////////////

  FFResult SetFloatParameter(unsigned int dwIndex, float value) override;
  float GetFloatParameter(unsigned int index) override;
  FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
  FFResult InitGL(const FFGLViewportStruct *vp) override;
  FFResult DeInitGL() override;

  FFResult SetTime(double time) override;
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
  float m_scaleX;
  float m_scaleY;
  float m_uniformScale;
  float m_translateX;
  float m_translateY;
  float m_rotate;
  bool m_fillEnabled;
  bool m_borderEnabled;
  float m_borderOffsetX;
  float m_borderOffsetY;
  float m_borderColorR;
  float m_borderColorG;
  float m_borderColorB;
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
