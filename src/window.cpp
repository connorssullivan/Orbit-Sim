#include "window.h"


Window::Window() : m_deltaTime(0.0f), m_lastTime(0.0f),
 m_yaw(-90.0f), m_pitch(0.0f), m_lastX(400.0f), m_lastY(300.0f),
 m_firstMouse(true), m_fov(45.f),m_camera()
{
    if (!glfwInit())
    {
        std::cout << "Errow With Initalization\n";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
    #endif
    
    m_window = glfwCreateWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);

    if (!m_window) 
    {
        std::cout << "Error creating your window \n";
        glfwTerminate();
    }
    
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to load glad\n";
        glfwTerminate();
    }

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);  // set color
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSetFramebufferSizeCallback(m_window, CallBacks::frameBufferSizeCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Capture Cursor
    glfwSetWindowUserPointer(m_window, this);

    glfwSetCursorPosCallback(m_window, Window::mouse_callback);
    glfwSetScrollCallback(m_window, Window::scroll_callback);
}


Window::~Window()
{
    if (m_window)
        glfwDestroyWindow(m_window);
    glfwTerminate();
}


void Window::onMouse(double xpos, double ypos)
{
    if (!m_mouseCaptured)
        return;
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return; // ImGui is using the mouse

    if (m_firstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos;

    m_lastX = xpos;
    m_lastY = ypos;

    m_camera.ProcessMouseMovement(xoffset, yoffset);
}

void Window::onScroll(double yoffset)
{
    m_camera.ProcessMouseScroll(yoffset);
}


void m_onMouse(double xpos, double ypos);
void m_onScroll(double yoffset);

void Window::processInput() {
    static bool lastTab = false;
    bool tab = glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (tab && !lastTab)
    {
        m_mouseCaptured = !m_mouseCaptured;
        glfwSetInputMode(
            m_window,
            GLFW_CURSOR,
            m_mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
        );
        m_firstMouse = true;
    }

    lastTab = tab;

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard)
        return;

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    const float cameraSpeed = 5.0f * m_deltaTime;
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::CameraMovement::FORWARD, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::CameraMovement::BACKWARD, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::CameraMovement::LEFT, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::CameraMovement::RIGHT, m_deltaTime);

}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self)
        self->onMouse(xpos, ypos);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self)
        self->onScroll(yoffset);
}
