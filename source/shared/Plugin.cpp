//
//  Plugin.cpp
//  VoronoiGenerator
//

#include "Plugin.h"

void PluginBase::RegisterParam(FloatParameter &param,
                               FFParamType type,
                               const char *name) {
  SetParamInfo(_params.size(), name, type, param.GetFloatValue());
  _params.push_back(&param);
}

void PluginBase::RegisterParam(BoolParameter &param,
                               const char *name) {
  SetParamInfo(_params.size(),
               name,
               FF_TYPE_BOOLEAN,
               param.GetValue());
  _params.push_back(&param);
}

void PluginBase::RegisterParam(ActionParameter &param,
                               const char *name) {
  SetParamInfo(_params.size(),
               name,
               FF_TYPE_EVENT,
               false);
  _params.push_back(&param);
}

ParameterBase* PluginBase::GetParam(unsigned int i) {
  if (i >= static_cast<unsigned int>(_params.size())) {
    return nullptr;
  }
  return _params[static_cast<std::size_t>(i)];
}

FFResult PluginBase::SetFloatParameter(unsigned int dwIndex,
                                       float value) {
  auto param = GetParam(dwIndex);
  return param ? param->SetFloatValue(value) : FF_FAIL;
}

float PluginBase::GetFloatParameter(unsigned int index) {
  auto param = GetParam(index);
  return param ? param->GetFloatValue() : 0.0f;
}

FFResult PluginBase::SetTextParameter(unsigned int index,
                                      const char *value) {
  auto param = GetParam(index);
  return param ? param->SetTextValue(value) : FF_FAIL;
}

const std::size_t displayBufferSize = 15;
static char displayBuffer[displayBufferSize];

char* PluginBase::GetTextParameter(unsigned int index) {
  auto param = GetParam(index);
  return param ? param->GetTextValue() : nullptr;
}

char* PluginBase::GetParameterDisplay(unsigned int index) {
  auto param = GetParam(index);
  if (!param) {
    return nullptr;
  }
  param->GetDisplayText(displayBuffer, displayBufferSize);
  return displayBuffer;
}



