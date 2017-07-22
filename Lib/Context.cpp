#include "Context.h"
#include <kvs/Message>


namespace kvs
{

namespace egl
{

Context::Context( kvs::egl::Display& display ):
    m_handle( NULL ),
//    m_config( display ),
//    m_surface( display ),
    m_display( display )
{
}

Context::~Context()
{
    this->destroy();
}

//bool Context::create( EGLint width, EGLint height )
bool Context::create( kvs::egl::Config& config )
{
    // Create an appropriate EGL frame buffer configuration.
/*
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

    if ( !m_config.create( config_attribs ) )
    {
        kvsMessageError( "Failed to choose configuration." );
        return false;
    }
*/
    // Create a drawing surface.
/*
    const EGLint surface_attribs[] = {
        EGL_WIDTH,  width,
        EGL_HEIGHT, height,
        EGL_NONE,
    };

    if ( !m_surface.create( m_config, surface_attribs ) )
    {
        kvsMessageError( "Cannot create drawing surface." );
        return false;
    }
*/
    // Bind the API
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( status = eglBindAPI( EGL_OPENGL_API ) );
    if ( status == EGL_FALSE )
    {
        kvsMessageError( "API binding failed" );
        return false;
    }

    // Create a context and make it current
//    KVS_EGL_CALL( m_handle = eglCreateContext( m_display.handle(), m_config.handle(), EGL_NO_CONTEXT, NULL ) );
    KVS_EGL_CALL( m_handle = eglCreateContext( m_display.handle(), config.handle(), EGL_NO_CONTEXT, NULL ) );

    return this->isValid();
}

void Context::destroy()
{
    this->releaseCurrent();

    if ( m_handle )
    {
        KVS_EGL_CALL( eglDestroyContext( m_display.handle(), m_handle ) );
        m_handle = NULL;
    }

    m_surface.destroy();
}

bool Context::makeCurrent( kvs::egl::Surface& surface )
{
    EGLBoolean status = EGL_FALSE;
//    KVS_EGL_CALL( status = eglMakeCurrent( m_display.handle(), m_surface.handle(), m_surface.handle(), m_handle ) );
    KVS_EGL_CALL( status = eglMakeCurrent( m_display.handle(), surface.handle(), surface.handle(), m_handle ) );
    if ( status == EGL_FALSE )
    {
        kvsMessageError( "Cannot bind drawing surface." );
        return false;
    }

    return true;
}

void Context::releaseCurrent()
{
    KVS_EGL_CALL( eglMakeCurrent( m_display.handle(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT ) );
}

bool Context::swapBuffers( kvs::egl::Surface& surface )
{
    EGLBoolean status = EGL_FALSE;
//    KVS_EGL_CALL( status = eglSwapBuffers( m_display.handle(), m_surface.handle() ) );
    KVS_EGL_CALL( status = eglSwapBuffers( m_display.handle(), surface.handle() ) );
    return status == EGL_TRUE;
}

bool Context::swapInterval( EGLint interval )
{
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( eglSwapInterval( m_display.handle(), interval ) );
    return status == EGL_TRUE;
}

} // end of namespace egl

} // end of namespace kvs
