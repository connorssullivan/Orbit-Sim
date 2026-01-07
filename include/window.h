#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "config.h"
#include "callbacks.h"
#include "camera.h"


class Window
{
    private:
        GLFWwindow* m_window;

        float m_deltaTime; 
        float m_lastTime;
        float m_yaw;
        float m_pitch;
        float m_lastX;
        float m_lastY;
        float m_fov;
        bool m_firstMouse;

        Camera m_camera;


        void onMouse(double xpos, double ypos);
        void onScroll(double yoffset);


    public:
        // Constructers
        Window();
        ~Window();

        // Getters
        GLFWwindow* get() const {return m_window;}
        bool shouldClose() const {return glfwWindowShouldClose(m_window);}
        void swapBuffers() const {glfwSwapBuffers(m_window);}
        void pollEvents() const {glfwPollEvents();}
        float getLastTime() const {return m_lastTime;};
        float getDeltaTime() const {return m_deltaTime;};
        Camera& getCamera() {return m_camera;}

        // setters
        void setLastTime(const float time) {m_lastTime = time;};
        void setDeltaTime(const float time) {m_deltaTime = time;};

        void processInput();

        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};