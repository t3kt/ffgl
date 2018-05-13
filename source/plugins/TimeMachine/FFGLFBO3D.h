#pragma once

#include <FFGL.h>

//FFGLTextureStruct (for ProcessOpenGLStruct)
typedef struct FFGLTexture3DStructTag : public FFGLTextureStructTag
{
  FFUInt32 Depth;
  FFUInt32 HardwareDepth;
} FFGLTexture3DStruct;

class FFGLFBO3D
{
public:
  FFGLFBO3D();


  int Create(int width, int height, int depth);
  int BindAsRenderTarget();
  int UnbindAsRenderTarget(GLuint hostFbo);

  FFGLTexture3DStruct GetTextureInfo();
  void FreeResources();

  GLuint GetWidth() { return m_width; }
  GLuint GetHeight() { return m_height; }
  GLuint GetDepth() { return m_depth; }
  GLuint GetFBOHandle() { return m_fboHandle; }

protected:
  GLuint m_width;
  GLuint m_height;
  GLuint m_depth;
  GLuint m_glWidth;
  GLuint m_glHeight;
  GLuint m_glDepth;
  GLuint m_glPixelFormat;
  GLuint m_glTextureTarget;
  GLuint m_glTextureHandle;
  GLuint m_fboHandle;
  GLuint m_depthBufferHandle;
};
