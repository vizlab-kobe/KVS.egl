#pragma once

#include "EGL.h"
#include "Display.h"
#include "Context.h"
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
    kvs::egl::Display m_display; ///< EGL display
    kvs::egl::Context m_context; ///< EGL rendering context

public:
    ScreenBase();
    virtual ~ScreenBase();

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
