#include "Surface.h"


namespace kvs
{

namespace egl
{

Surface::Surface( kvs::egl::Display& display ):
    m_handle( NULL ),
    m_display( display )
{
}

Surface::~Surface()
{
    this->destroy();
}

bool Surface::create( const kvs::egl::Config& config, const EGLint* attribs )
{
    KVS_EGL_CALL( m_handle = eglCreatePbufferSurface( m_display.handle(), config.handle(), attribs ) );
    return m_surface != EGL_NO_SURFACE;
}

void Surface::destroy()
{
    if ( m_handle )
    {
        KVS_EGL_CALL( eglDestroySurface( m_display.handle(), m_handle ) );
        m_handle = NULL;
    }
}

} // end of namespace egl

} // end of namespace kvs
