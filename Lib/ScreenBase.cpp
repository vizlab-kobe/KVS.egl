#include "ScreenBase.h"
#include <kvs/ColorImage>
#include <kvs/OpenGL>
#include <iostream>
#include <string>

namespace kvs
{

namespace egl
{

ScreenBase::ScreenBase():
    m_context( NULL )
{
    // Initialize EGL
    m_display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    if ( m_display == EGL_NO_DISPLAY )
    {
        std::cerr << "Error : Cannot get display." << std::endl;
        return;
    }

    EGLint major, minor;
    if ( !eglInitialize( m_display, &major, &minor ) )
    {
	errorMessage( "Initialization failed." );
	return;
    }
}

ScreenBase::~ScreenBase()
{
    if ( m_display )
    {
        eglTerminate( m_display );
        m_display = NULL;
        m_context = NULL;
	m_surface = NULL;
    }
}

kvs::ColorImage ScreenBase::capture() const
{
    const size_t width = BaseClass::width();
    const size_t height = BaseClass::height();
    kvs::ValueArray<kvs::UInt8> buffer( width * height * 3 );
    // kvs::ValueArray<kvs::UInt8> buffer( width * height * 4 );

    kvs::OpenGL::SetReadBuffer( GL_FRONT );
    kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data() );
    // kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer );

    // RGBA to RGB
    // kvs::ValueArray<kvs::UInt8> pixels( width * height * 3 );
    // for ( size_t i = 0; i < width * height; i++ )
    // {
    //     pixels[ 3 * i + 0 ] = buffer[ 4 * i + 0 ];
    //     pixels[ 3 * i + 1 ] = buffer[ 4 * i + 1 ];
    //     pixels[ 3 * i + 2 ] = buffer[ 4 * i + 2 ];
    // }

    return kvs::ColorImage( width, height, buffer );
}

void ScreenBase::displayInfo()
{
    std::cout << "EGL_CLIENT_APIS : ";
    std::cout << eglQueryString( m_display, EGL_CLIENT_APIS ) << std::endl;
    std::cout << "EGL_VENDOR : ";
    std::cout << eglQueryString( m_display, EGL_VENDOR ) << std::endl;
    std::cout << "EGL_VERSION : ";
    std::cout << eglQueryString( m_display, EGL_VERSION ) << std::endl;
    std::cout << "EGL_EXTENSIONS : ";
    std::cout << eglQueryString( m_display, EGL_EXTENSIONS ) << std::endl;
}

void ScreenBase::draw()
{
    if ( !m_context ) { this->create(); }
    this->paintEvent();
}

void ScreenBase::create()
{
    // Select an appropriate configuration
    EGLint numConfigs;
    EGLConfig eglCfg;
    const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE,  8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE,   8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };    
    if ( !eglChooseConfig( m_display, configAttribs, &eglCfg, 1, &numConfigs ) )
    {
	errorMessage( "Configuration choice failed." );
	return;
    }

    // Create a surface
    const EGLint pbufferAttribs[] = {
        EGL_WIDTH,  BaseClass::width(),
        EGL_HEIGHT, BaseClass::height(),
        EGL_NONE,
    };
    m_surface = eglCreatePbufferSurface( m_display, eglCfg, pbufferAttribs );
    if ( m_surface == EGL_NO_SURFACE )
    {
	errorMessage( "Cannot create surface." );
	return;
    }

    // Bind the API
    if ( !eglBindAPI( EGL_OPENGL_API ) )
    {
	errorMessage( "API binding failed" );
	return;
    }

    // Create a context and make it current
    m_context = eglCreateContext( m_display, eglCfg, EGL_NO_CONTEXT, NULL );
    if ( !m_context )
    {
	errorMessage( "Cannot create EGL context." );
	return;
    }
    if ( !eglMakeCurrent( m_display, m_surface, m_surface, m_context ) )
    {
        errorMessage( "Cannot bind surface." );
        return;
    }

    this->initializeEvent();
}

void ScreenBase::show()
{
    if ( !m_context ) { this->create(); }
}

void ScreenBase::redraw()
{
    this->draw();
}

void ScreenBase::errorMessage( const char* msg )
{
    std::cerr << "Error : " << msg << std::endl;
    EGLint code = eglGetError();
    std::string err_msg;
    switch ( code )
    {
        case 0x3000 : err_msg = "EGL_SUCCESS"; break;
        case 0x3001 : err_msg = "EGL_NOT_INITIALIZED"; break;
        case 0x3002 : err_msg = "EGL_BAD_ACCESS"; break;
        case 0x3003 : err_msg = "EGL_BAD_ALLOC"; break;
        case 0x3004 : err_msg = "EGL_BAD_ATTRIBUTE"; break;
        case 0x3005 : err_msg = "EGL_BAD_CONFIG"; break;
        case 0x3006 : err_msg = "EGL_BAD_CONTEXT"; break;
        case 0x3007 : err_msg = "EGL_BAD_CURRENT_SURFACE"; break;
        case 0x3008 : err_msg = "EGL_BAD_DISPLAY"; break;
        case 0x3009 : err_msg = "EGL_BAD_MATCH"; break;
        case 0x300A : err_msg = "EGL_BAD_NATIVE_PIXMAP"; break;
        case 0x300B : err_msg = "EGL_BAD_NATIVE_WINDOW"; break;
        case 0x300C : err_msg = "EGL_BAD_PARAMETER"; break;
        case 0x300D : err_msg = "EGL_BAD_SURFACE"; break;
        default : err_msg = "UNKNOWN_ERROR_CODE"; break;
    }
    std::cerr << "Code  : " << err_msg << " (0x" << std::hex << code << ")" << std::endl;
    exit( 1 );
}

} // end of namespace egl

} // end of namespace kvs
