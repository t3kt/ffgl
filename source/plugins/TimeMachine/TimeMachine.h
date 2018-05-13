#pragma once

#include <FFGLShader.h>
#include <FFGLPluginSDK.h>
#include <string>

class TimeMachine : public CFreeFrameGLPlugin {
public:
  TimeMachine();
  ~TimeMachine();

  ///////////////////////////////////////////////////
  // FreeFrame plugin methods
  ///////////////////////////////////////////////////

  FFResult SetFloatParameter(unsigned int dwIndex, float value) override;
  float GetFloatParameter(unsigned int index) override;
  FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
  FFResult InitGL(const FFGLViewportStruct *vp) override;
  FFResult DeInitGL() override;

  //The mixer short name
  const char * GetShortName() override { static const char* sname = "TiMa"; return sname; }

  ///////////////////////////////////////////////////
  // Factory method
  ///////////////////////////////////////////////////

  static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
  {
    *ppOutInstance = new TimeMachine();
    if (*ppOutInstance != NULL)
      return FF_SUCCESS;
    return FF_FAIL;
  }

private:
  // Parameters
  float _mixVal;

  FFGLShader _shader;
  GLint _inputTextureLocation1;
  GLint _inputTextureLocation2;
  GLint _mixValLocation;
};
