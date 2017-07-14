#include "Context.h"
#include <kvs/Message>


namespace kvs
{

namespace egl
{

Context::Context():
    m_handle( NULL ),
    m_display( NULL ),
    m_surface( NULL ),
    m_config( NULL )
{
}

Context::~Context()
{
    this->destroy();
}

bool Context::create( EGLint width, EGLint height )
{
    // Create EGL display connection.
    KVS_EGL_CALL( m_display = eglGetDisplay( EGL_DEFAULT_DISPLAY ) );
    if ( m_display == EGL_NO_DISPLAY )
    {
        kvsMessageError( "Cannot create EGL display connection." );
        return false;
    }

    // Initialize EGL display connection.
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( status = eglInitialize( m_display, NULL, NULL ) );
    if ( status == EGL_FALSE )
    {
        kvsMessageError( "Failed to initialize EGL." );
        return false;
    }

    // Create an appropriate EGL frame buffer configuration.
    EGLint nconfigs;
    const EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE,  8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE,   8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };
    KVS_EGL_CALL( status = eglChooseConfig( m_display, config_attribs, &m_config, 1, &nconfigs ) );
    if ( status == EGL_FALSE )
    {
        kvsMessageError( "Failed to choose configuration." );
        return false;
    }

    // Create a drawing surface.
    const EGLint surface_attribs[] = {
        EGL_WIDTH,  width,
        EGL_HEIGHT, height,
        EGL_NONE,
    };
    KVS_EGL_CALL( m_surface = eglCreatePbufferSurface( m_display, m_config, surface_attribs ) );
    if ( m_surface == EGL_NO_SURFACE )
    {
        kvsMessageError( "Cannot create drawing surface." );
        return false;
    }

    // Bind the API
    KVS_EGL_CALL( status = eglBindAPI( EGL_OPENGL_API ) );
    if ( status == EGL_FALSE )
    {
        kvsMessageError( "API binding failed" );
        return false;
    }

    // Create a context and make it current
    KVS_EGL_CALL( m_handle = eglCreateContext( m_display, m_config, EGL_NO_CONTEXT, NULL ) );

    return this->isValid();
}

void Context::destroy()
{
    if ( m_display )
    {
        this->releaseCurrent();

        if ( m_handle )
        {
            KVS_EGL_CALL( eglDestroyContext( m_display, m_handle ) );
            m_handle = NULL;
        }

        if ( m_surface )
        {
            KVS_EGL_CALL( eglDestroySurface( m_display, m_surface ) );
            m_surface = NULL;
        }

        KVS_EGL_CALL( eglTerminate( m_display ) );
        m_display = NULL;
    }
}

bool Context::makeCurrent()
{
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( status = eglMakeCurrent( m_display, m_surface, m_surface, m_handle ) );
    if ( status == EGL_FALSE )
    {
        kvsMessageError( "Cannot bind drawing surface." );
        return false;
    }

    return true;
}

void Context::releaseCurrent()
{
    KVS_EGL_CALL( eglMakeCurrent( m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT ) );
}

bool Context::swapBuffers()
{
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( status = eglSwapBuffers( m_display, m_surface ) );
    return status == EGL_TRUE;
}

bool Context::swapInterval( EGLint interval )
{
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( eglSwapInterval( m_display, interval ) );
    return status == EGL_TRUE;
}

} // end of namespace egl

} // end of namespace kvs
