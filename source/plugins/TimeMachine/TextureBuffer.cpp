#include "TextureBuffer.h"
#include "../../lib/ffgl/utilities/utilities.h"
#include <string>

const std::size_t bytesPerValue = 3;

TextureBuffer::TextureBuffer(FFUInt32 width,
                             FFUInt32 height,
                             FFUInt32 depth)
: _offset(0.0f) {
  _valid = true;
  if (!_fbo.Create(width, height, depth)) {
    _valid = false;
    return;
  }
}

TextureBuffer::~TextureBuffer() {
  if (_valid) {
    _fbo.FreeResources();
  }
}

void TextureBuffer::pushFrame(const FFGLTextureStruct &sourceTexture) {
  if (!_valid) {
    return;
  }
  //...
}
