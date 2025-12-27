#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.h"
#include "callbacks.h"
#include "window.h"
#include "renderer.h"

#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "shader.h"
#include "vertexArray.h"
#include "texture.h"
#include "camera.h"

Camera camera;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastTime = 0.0f; // Time of last frame

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse {true};

float fov {45.f};
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() {

    Window window;
    glEnable(GL_DEPTH_TEST);

    // Load Shaders
    Shader shader;
    shader.SetShader(ShaderPaths::VERTEX_SHADER_PATH, ShaderPaths::FRAGMENT_SHADER_PATH);

    Shader bgShader;
    bgShader.SetShader(ShaderPaths::BG_VERTEX_SHADER_PATH, ShaderPaths::BG_FRAFMENT_SHADER_PATH);

    float bgVertices[] = {
        // pos (NDC)      // tex
        -1.0f, -1.0f,    0.0f, 0.0f,
        1.0f, -1.0f,    1.0f, 0.0f,
        1.0f,  1.0f,    1.0f, 1.0f,
        -1.0f,  1.0f,    0.0f, 1.0f
    };

    unsigned int bgIndices[] = {
        0, 1, 2,
        2, 3, 0
    };


    float vertices[] = {
        // Front face (red)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        // Back face (green)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        // Left face (blue)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        // Right face (yellow)
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        // Top face (cyan)
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        // Bottom face (magenta)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    };



    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,       // front
        4, 5, 6,  6, 7, 4,       // back
        8, 9,10, 10,11, 8,       // left
        12,13,14, 14,15,12,       // right
        16,17,18, 18,19,16,       // top
        20,21,22, 22,23,20        // bottom
    };

    // Background
    VertexBuffer bgVB(bgVertices, sizeof(bgVertices));
    IndexBuffer bgIB(bgIndices, sizeof(bgIndices)/sizeof(unsigned int));

    VertexBufferLayout bgLayout;
    VertexArray bgVA;

    bgLayout.push<float>(2);
    bgLayout.push<float>(2);

    bgVA.Bind();
    bgIB.Bind();
    bgVA.AddBuffer(bgVB, bgLayout);

    // Set up VBO/VAO/EBO
    VertexBuffer vb(vertices, sizeof(vertices));
    IndexBuffer ib(indices, sizeof(indices)/sizeof(unsigned int));

    VertexBufferLayout layout;
    VertexArray va;

    layout.push<float>(3);
    layout.push<float>(2);

    va.Bind();
    ib.Bind();
    va.AddBuffer(vb, layout);
    

    // --------------------------------------------------
    // Texture Loading
    // --------------------------------------------------
    Texture earthTex(Textures::EARTH_TEX);
    Texture bgTex(Textures::STARS_TEX);
    
    ///////////////////////////////
    //      Main Render Loop
    //////////////////////////////
    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Timing 
        float currentTime = glfwGetTime();
        window.setDeltaTime(currentTime - window.getLastTime());
        window.setLastTime(currentTime);

        // MVP
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // Our camera is -3 to start
        model = glm::rotate(model, currentTime * 0.5f, glm::vec3(0, 1, 0));

        glm::mat4 view = window.getCamera().GetViewMatrix();

        glm::mat4 projection = glm::perspective(
            glm::radians(window.getCamera().m_zoom),
            (float)Config::SCREEN_WIDTH / Config::SCREEN_HEIGHT,
            0.1f,
            100.f
        );

        // ------ Background -------
        glDisable(GL_DEPTH_TEST);
        bgShader.Use();

        bgShader.SetInt("uBackground", 0);

        static glm::vec3 lastCamPos = window.getCamera().m_positions;
        glm::vec3 delta = window.getCamera().m_positions - lastCamPos;
        lastCamPos = window.getCamera().m_positions;

        // Only strafe affects background
        float strafe = glm::dot(delta, window.getCamera().m_right);

        static glm::vec2 bgOffset(0.0f);
        bgOffset.x += strafe * 0.05f;

        // Add rotation
        float yawOffset = glm::radians(window.getCamera().m_yaw) * 0.15f;

        glm::vec2 finalOffset = bgOffset;
        finalOffset.x += yawOffset;

        // Zoom scaling
        float bgZoom = 1.0f + (45.0f - window.getCamera().m_zoom) * 0.02f;
        bgZoom = glm::clamp(bgZoom, 0.7f, 2.5f);

        bgShader.SetVec2("uOffset", finalOffset);
        bgShader.SetFloat("uZoom", bgZoom);





        glActiveTexture(GL_TEXTURE0);
        bgTex.Bind(0);

        bgVA.Bind();
        bgIB.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // ------ Cube -------
        glEnable(GL_DEPTH_TEST);
        shader.Use();

        shader.SetInt("uTexture", 0);

        shader.SetMat4("uModel", model);
        shader.SetMat4("uView", view);
        shader.SetMat4("uProjection", projection);

        GLCall(glActiveTexture(GL_TEXTURE0));
        earthTex.Bind(0);

        va.Bind();
        ib.Bind();

        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

        window.processInput();

        window.swapBuffers();
        window.pollEvents();
    }
    
    
    
    return 0;
}



