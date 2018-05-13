//
//  Parameter.h
//  VoronoiGenerator
//

#pragma once

#include <cstddef>
#include <functional>
#include "Common.h"
#include <FFGLPluginSDK.h>
#include <iostream>

class ParameterBase {
public:
  ParameterBase() {}
  virtual ~ParameterBase() {}
  virtual void GetDisplayText(char* buffer, std::size_t size) = 0;
  virtual FFResult SetFloatValue(float value) { return FF_FAIL; }
  virtual FFResult SetTextValue(const char* value) { return FF_FAIL; }
  virtual float GetFloatValue() { return 0.0f; }
  virtual char* GetTextValue() { return (char*) FF_FAIL; }
};

class FloatParameter : public ParameterBase {
public:
  FloatParameter(float value, float rangeLow, float rangeHigh);

  void GetDisplayText(char* buffer, std::size_t size) override;
  FFResult SetFloatValue(float value) override;
  float GetFloatValue() override { return _normalizedValue; }

  float GetValue() const;
private:
  float _rangeLow;
  float _rangeHigh;
  float _normalizedValue;
};

std::ostream& operator<<(std::ostream& os,
                         const FloatParameter& param);

class BoolParameter : public ParameterBase {
public:
  BoolParameter(bool value,
                const char* onText,
                const char* offText)
  : _value(value)
  , _onText(onText)
  , _offText(offText) {}

  void GetDisplayText(char* buffer, std::size_t size) override;
  FFResult SetFloatValue(float value) override;
  float GetFloatValue() override { return _value ? 1.0f : 0.0f; }

  bool GetValue() const { return _value; }

private:
  bool _value;
  const char* _onText;
  const char* _offText;
};

std::ostream& operator<<(std::ostream& os,
                         const BoolParameter& param);

class ActionParameter : public ParameterBase {
public:
  explicit ActionParameter(std::function<void(void)> action)
  : _action(action) {}
  void GetDisplayText(char *buffer, std::size_t size) override;

  FFResult SetFloatValue(float value) override;
private:
  std::function<void(void)> _action;
};

