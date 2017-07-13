#pragma once
#include "Call.h"
#include <kvs/String>
#include <string>


namespace kvs
{

namespace egl
{

inline std::string Description()
{
    const std::string description( "EGL - An interface between OpenGL and the native windowing system." );
    return description;
}

} // end of namespace osmesa

} // end of namespace kvs
