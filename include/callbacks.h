#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace CallBacks
{
    // Name: Frame Buffer Size Callback
    // Desc: Used to resize the window
    void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
}