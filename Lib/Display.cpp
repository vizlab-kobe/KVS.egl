#include "Display.h"


namespace kvs
{

namespace egl
{

Display::Display():
    m_display( NULL ),
    m_major_version( 0 ),
    m_minor_version( 1 )
{
}

Display::~Display()
{
}

bool Display::create( EGLNativeDisplayType display_id )
{
    KVS_EGL_CALL( m_display = eglGetDisplay( display_id ) );
    return m_display != EGL_NO_DISPLAY;
}

bool Display::initialize()
{
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( status = eglInitialize( m_display, &m_major_version, &m_minor_version ) );
    return status != EGL_FALSE;
}

void Display::terminate()
{
    KVS_EGL_CALL( eglTerminate( m_display ) );
    m_display = NULL;
}

} // end of namespace egl

} // end of namespace kvs
