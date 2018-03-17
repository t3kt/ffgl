#include <FFGL.h>
#include <FFGLLib.h>
#include "TimeMachine.h"
#include "../../lib/ffgl/utilities/utilities.h"

#define FFParam_MixVal (0)

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo
PluginInfo (
            TimeMachine::CreateInstance,        // Create method
            "TIMA",                // Plugin unique ID
            "Time Machine",              // Plugin name
            1,                     // API major version number
            500,                // API minor version number
            1,                  // Plugin major version number
            000,                // Plugin minor version number
            FF_EFFECT,              // Plugin type
            "Blend two videos by adding using one to determine " \
              "time offsets in the other.",// Plugin description
            "Time Machine plugin by tekt@optexture.com"  // About
);

static const std::string vertexShaderCode = STRINGIFY
(
//...
);

static const std::string fragmentShaderCode = STRINGIFY
(
 //...
);

TimeMachine::TimeMachine()
: CFreeFrameGLPlugin(),
_inputTextureLocation1(-1),
_inputTextureLocation2(-1),
_mixValLocation(-1) {

  // Input properties
  SetMinInputs(2);
  SetMaxInputs(2);

  // Parameters
  SetParamInfo(FFParam_MixVal, "Mixer Value", FF_TYPE_STANDARD, 0.5f);
  _mixVal = 0.5f;
}

TimeMachine::~TimeMachine() {
}

FFResult TimeMachine::InitGL(const FFGLViewportStruct *vp) {
  //initialize gl shader
  m_shader.Compile(vertexShaderCode,fragmentShaderCode);

  //activate our shader
  m_shader.BindShader();

  _inputTextureLocation1 = _shader.FindUniform("textureDest");
  _inputTextureLocation2 = _shader.FindUniform("textureSrc");
  _mixValLocation = _shader.FindUniform("mixVal");

  //the 0 means that the 'inputTexture' in
  //the shader will use the texture bound to GL texture unit 0
  glUniform1i(m_inputTextureLocation1, 0);
  glUniform1i(m_inputTextureLocation2, 1);

  _shader.UnbindShader();

  return FF_SUCCESS;
}

FFResult TimeMachine::DeInitGL() {
  _shader.FreeGLResources();

  return FF_SUCCESS;
}

