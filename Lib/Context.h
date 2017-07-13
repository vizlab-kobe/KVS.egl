#pragma once
#include "EGL.h"

namespace kvs
{

namespace egl
{

class Context
{
private:
    EGLContext m_handle; // EGL rendering context
    EGLDisplay m_display; // EGL display
    EGLSurface m_surface; // EGL surface

public:
    Context();
    ~Context();

    void* handle() { return m_handle; }
    bool isValid() const { return m_handle != NULL; }

    bool create( EGLint width, EGLint height );
    void destroy();
    bool makeCurrent();
    void releaseCurrent();
};

} // end of namespace egl

} // end of namespace kvs
