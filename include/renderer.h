#pragma once

#include <glad/glad.h>
#include <iostream>

#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define DEBUG_BREAK() __builtin_trap()
#else
    #define DEBUG_BREAK()
#endif

#define ASSERT(x) if (!(x)) DEBUG_BREAK()
#define GLCall(x) GLClearError(); \
    x; \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
