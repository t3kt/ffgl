//
//  Common.cpp
//  VoronoiGenerator
//

#include "Common.h"
#include <float.h>
#include <cmath>

float mapRange(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

  if (std::fabs(inputMin - inputMax) < FLT_EPSILON) {
    return outputMin;
  } else {
    float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

    if( clamp ){
      if(outputMax < outputMin){
        if( outVal < outputMax )outVal = outputMax;
        else if( outVal > outputMin )outVal = outputMin;
      }else{
        if( outVal > outputMax )outVal = outputMax;
        else if( outVal < outputMin )outVal = outputMin;
      }
    }
    return outVal;
  }

}
