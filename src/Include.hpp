#pragma once

#ifdef PLATFORM_WINDOWS
    #include <Windows.h>
    #include <GL/gl.h>
#elif defined(PLATFORM_MACOS)
    #include <OpenGL/gl.h>
#elif defined(PLATFORM_LINUX)
    #include <GL/gl.h>
#endif