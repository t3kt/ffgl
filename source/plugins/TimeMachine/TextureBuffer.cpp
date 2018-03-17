#include "TextureBuffer.h"
#include "../../lib/ffgl/utilities/utilities.h"
#include <string>

static const std::string vertexShaderCode = STRINGIFY
(
 void main() {
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_TexCoord[1] = gl_MultiTexCoord1;
   gl_FrontColor = gl_Color;
 }
);
static const std::string fragmentShaderCode = STRINGIFY
(
 uniform sampler3D textureBuffer;
 uniform sampler2D textureNewFrame;
 uniform float offset;

 void main() {

 }
);

TextureBuffer::TextureBuffer(FFUInt32 width,
                             FFUInt32 height,
                             FFUInt32 depth)
: _offset(0.0f)
, _depthLocation(-1)
, _offsetLocation(-1) {
  _valid = true;
  if (!_fbo.Create(width, height, depth)) {
    _valid = false;
    return;
  }
  if (!_shader.Compile(vertexShaderCode, fragmentShaderCode)) {
    _fbo.FreeResources();
    _valid = false;
    return;
  }
  _valid = !!_fbo.Create(width, height, depth);
  
  _shader.BindShader();
  _textureBufferLocation = _shader.FindUniform("textureBuffer");
  _textureNewFrameLocation = _shader.FindUniform("textureNewFrame");
  _offsetLocation = _shader.FindUniform("offset");

  glUniform1f(_offsetLocation, 0.0);

  _shader.UnbindShader();
}

TextureBuffer::~TextureBuffer() {
  if (_valid) {
    _fbo.FreeResources();
    _shader.FreeGLResources();
  }
}

void TextureBuffer::pushFrame(const FFGLTextureStruct &sourceTexture) {
  if (!_valid) {
    return;
  }
  //...
}
