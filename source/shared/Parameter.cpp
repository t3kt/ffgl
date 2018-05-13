//
//  Parameter.cpp
//  VoronoiGenerator
//

#include "Parameter.h"
#include <cstdio>
#include <cstring>

FloatParameter::FloatParameter(float value,
                               float rangeLow, float rangeHigh)
: _rangeLow(rangeLow)
, _rangeHigh(rangeHigh)
, _normalizedValue(mapRange(value, rangeLow, rangeHigh, 0.0f, 1.0f, true)) {
}

void FloatParameter::GetDisplayText(char *buffer, std::size_t size) {
  std::memset(buffer, 0, size);
  std::snprintf(buffer, size,
                "%f", GetValue());
}

FFResult FloatParameter::SetFloatValue(float value) {
  _normalizedValue = value;
  return FF_SUCCESS;
}

float FloatParameter::GetValue() const {
  return mapRange(_normalizedValue, 0.0f, 1.0f,
                  _rangeLow, _rangeHigh, true);
}

FFResult BoolParameter::SetFloatValue(float value) {
  _value = value > 0.5f;
  return FF_SUCCESS;
}

void BoolParameter::GetDisplayText(char *buffer, std::size_t size) {
  std::memset(buffer, 0, size);
  std::strncpy(buffer, _value ? _onText : _offText, size);
}

void ActionParameter::GetDisplayText(char *buffer, std::size_t size) {
  std::memset(buffer, 0, size);
  std::strncpy(buffer, "", size);
}

FFResult ActionParameter::SetFloatValue(float value) {
  if (value > 0.5f) {
    _action();
  }
  return FF_SUCCESS;
}
