#pragma once

/* #include "OSMesa.h" */
#include <EGL/egl.h>
#include <kvs/ScreenBase>
#include <kvs/ValueArray>
#include <kvs/ColorImage>


namespace kvs
{

namespace egl
{

class ScreenBase : public kvs::ScreenBase
{
    typedef kvs::ScreenBase BaseClass;

private:

    /* OSMesaContext m_context; ///< OSMesa context */
    EGLDisplay m_display; // EGL display
    EGLContext m_context; // EGL context
    EGLSurface m_surface; // EGL surface
    /* kvs::ValueArray<kvs::UInt8> m_buffer; ///< frame buffer */
 
public:

    ScreenBase();
    virtual ~ScreenBase();

    /* const kvs::ValueArray<kvs::UInt8>& buffer() const { return m_buffer; } */
    kvs::ColorImage capture() const;
    void displayInfo();
    void draw();

    virtual void create();
    virtual void show();
    virtual void redraw();

    virtual void initializeEvent() {}
    virtual void paintEvent() {}

private:

    void errorMessage( const char* msg );
};

} // end of namespace egl

} // end of namepsace kvs
