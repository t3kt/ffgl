#pragma once

#include <FFGLShader.h>
#include "FFGLFBO3D.h"

class TextureBuffer {
public:
  TextureBuffer(FFUInt32 width, FFUInt32 height, FFUInt32 depth);
  ~TextureBuffer();

  bool valid() const { return _valid; }

  void pushFrame(const FFGLTextureStruct& sourceTexture);
private:
  bool _valid;
  FFGLFBO3D _fbo;
  GLuint _offset;
  FFGLShader _shader;
  GLint _textureBufferLocation;
  GLint _textureNewFrameLocation;
  GLint _offsetLocation;
};
