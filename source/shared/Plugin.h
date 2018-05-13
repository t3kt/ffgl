//
//  Plugin.h
//  VoronoiGenerator
//

#pragma once

#include "Common.h"
#include "Parameter.h"
#include <FFGLPluginSDK.h>
#include <vector>

class PluginBase : public CFreeFrameGLPlugin {
public:

  FFResult SetFloatParameter(unsigned int dwIndex, float value) override;
  float GetFloatParameter(unsigned int index) override;
  FFResult SetTextParameter(unsigned int index, const char *value) override;
  char* GetTextParameter(unsigned int index) override;
  char* GetParameterDisplay(unsigned int index) override;

protected:
  void RegisterParam(FloatParameter& param,
                     FFParamType type,
                     const char* name);
  void RegisterParam(BoolParameter& param, const char* name);
  void RegisterParam(ActionParameter& param, const char* name);
  ParameterBase* GetParam(unsigned int i);
private:
  std::vector<ParameterBase*> _params;
};
