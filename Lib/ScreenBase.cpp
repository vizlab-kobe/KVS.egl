#include "ScreenBase.h"
#include <kvs/ColorImage>
#include <kvs/OpenGL>
#include <iostream>
#include <string>


namespace kvs
{

namespace egl
{

ScreenBase::ScreenBase()
{
}

ScreenBase::~ScreenBase()
{
}

kvs::ColorImage ScreenBase::capture() const
{
    const size_t width = BaseClass::width();
    const size_t height = BaseClass::height();
    kvs::ValueArray<kvs::UInt8> buffer( width * height * 3 );

    kvs::OpenGL::SetReadBuffer( GL_FRONT );
    kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data() );
    return kvs::ColorImage( width, height, buffer );
}

void ScreenBase::displayInfo()
{
  /*
    EGLDisplay display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    std::cout << "EGL_CLIENT_APIS : ";
    std::cout << eglQueryString( display, EGL_CLIENT_APIS ) << std::endl;
    std::cout << "EGL_VENDOR : ";
    std::cout << eglQueryString( display, EGL_VENDOR ) << std::endl;
    std::cout << "EGL_VERSION : ";
    std::cout << eglQueryString( display, EGL_VERSION ) << std::endl;
    std::cout << "EGL_EXTENSIONS : ";
    std::cout << eglQueryString( display, EGL_EXTENSIONS ) << std::endl;
  */
}

void ScreenBase::draw()
{
    if ( !m_context.isValid() ) { this->create(); }
    this->paintEvent();
    m_context.swapBuffers();
}

void ScreenBase::create()
{
    // Create EGL context
    const size_t width = BaseClass::width();
    const size_t height = BaseClass::height();
    m_context.create( width, height );
    if ( !m_context.isValid() )
    {
        kvsMessageError( "Cannot create EGL context." );
        return;
    }

    // Bind the context to the buffer
    if ( !m_context.makeCurrent() )
    {
        kvsMessageError( "Cannot bind buffer." );
        return;
    }

    this->initializeEvent();
}

void ScreenBase::show()
{
    if ( !m_context.isValid() ) { this->create(); }
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
