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
#include "backGroundTexture.h"
#include "general/camera.h"
#include "planet.h"

Camera camera;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastTime = 0.0f; // Time of last frame

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse {true};

constexpr double TIME_SCALE = 0.01; // What universe sandbox uses
constexpr double G = 39.47841760435743; // 4π²
constexpr double PHYSICS_DT = 1.0 / 1000.0; // years per step
constexpr int PHYSICS_STEPS = 1; // speed-up factor

constexpr double DISTANCE_SCALE = 2.0; // AU → render units
constexpr double SIZE_SCALE = 10.0; 

float fov {45.f};
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() {

    Window window;
    glEnable(GL_DEPTH_TEST);

    // Load Shaders
    Shader planetShader;
    planetShader.SetShader(ShaderPaths::PLANET_VERTEX_SHADER_PATH, ShaderPaths::PLANET_FRAGMENT_SHADER_PATH);

    Shader bgShader;
    bgShader.SetShader(ShaderPaths::BG_VERTEX_SHADER_PATH, ShaderPaths::BG_FRAFMENT_SHADER_PATH);

    float bgVertices[] = {
        // pos     // tex
        -1.0f, -1.0f,    0.0f, 0.0f,
        1.0f, -1.0f,    1.0f, 0.0f,
        1.0f,  1.0f,    1.0f, 1.0f,
        -1.0f,  1.0f,    0.0f, 1.0f
    };

    unsigned int bgIndices[] = {
        0, 1, 2,
        2, 3, 0
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

    // ---------------- PLANETS ----------------
    // 

    // Velocitys
    double mercury_velocity   = std::sqrt(G / Solar::MERCURY_AU);
    double venus_velocity   = std::sqrt(G / Solar::VENUS_AU);
    double earth_velocity   = std::sqrt(G / Solar::EARTH_AU);
    // Moon orbital velocity around Earth (using Earth's mass, not Sun's)
    //double earth_moon_velocity = std::sqrt(G * Solar::EARTH_MASS / Solar::MOON_EARTH_AU);
    double mars_velocity = std::sqrt(G / Solar::MARS_AU);
    double saturn_velocity  = std::sqrt(G / Solar::SATURN_AU);
    double jupiter_velocity = std::sqrt(G / Solar::JUPITER_AU);
    double uranus_velocity   = std::sqrt(G / Solar::URANUS_AU);
    double neptune_velocity   = std::sqrt(G / Solar::NEPTUNE_AU);


    std::vector<Planet> planets;
    planets.reserve(12); // important: prevents reallocations

    // ---- Sun ----
    planets.emplace_back(
        ModelPaths::SUN_MODEL,
        glm::dvec3(0.0, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, 0.0),
        Solar::SUN_MASS,
        1.0,
        glm::radians(10.0f)
    );

    // ---- Mercury ----
    planets.emplace_back(
        ModelPaths::MERCURY_MODEL,
        glm::dvec3(Solar::MERCURY_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, mercury_velocity),
        Solar::MERCURY_MASS,
        Solar::MERCURY_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    // ---- Venus ----
    planets.emplace_back(
        ModelPaths::VENUS_MODEL,
        glm::dvec3(Solar::VENUS_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, venus_velocity),
        Solar::VENUS_MASS,
        Solar::VENUS_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    // ---- Earth ----
    planets.emplace_back(
        ModelPaths::EARTH_MODEL,
        glm::dvec3(Solar::EARTH_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, earth_velocity),
        Solar::EARTH_MASS,
        Solar::EARTH_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    glm::dvec3 moonOffset(Solar::MOON_EARTH_AU, 0.0, 0.0);
    glm::dvec3 moonPos = planets[3].position + moonOffset; // Earth index
    glm::dvec3 moonVel =
        planets[3].velocity +
        glm::dvec3(0.0, 0.0, Solar::moonOrbitalSpeed());

    /*planets.emplace_back(
        ModelPaths::EARTH_MOON_MODEL,
        moonPos,
        moonVel,
        Solar::MOON_MASS,
        Solar::MOON_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );*/


    // ---- Mars ----
    planets.emplace_back(
        ModelPaths::MARS_MODEL,
        glm::dvec3(Solar::MARS_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, mars_velocity),
        Solar::MARS_MASS,
        Solar::MARS_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    // ---- Jupiter ----
    planets.emplace_back(
        ModelPaths::JUIPITER_MODEL,
        glm::dvec3(Solar::JUPITER_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, jupiter_velocity),
        Solar::JUPITER_MASS,
        Solar::JUPITER_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    // ---- Saturn ----
    planets.emplace_back(
        ModelPaths::SATURN_MODEL,
        glm::dvec3(Solar::SATURN_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, saturn_velocity),
        Solar::SATURN_MASS,
        Solar::SATURN_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    // ---- Saturn Rings ---- (Just following satturns size and acceleration for now)
    planets.emplace_back(
        ModelPaths::SATURN_RING_MODEL,
        glm::dvec3(Solar::SATURN_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, saturn_velocity),
        0, //// No gravity
        Solar::SATURN_SIZE * 2 * SIZE_SCALE,
        glm::radians(20.0f)
    );

    // ---- Uranus ----
    planets.emplace_back(
        ModelPaths::URANUS_MODEL,
        glm::dvec3(Solar::URANUS_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, uranus_velocity),
        Solar::URANUS_MASS,
        Solar::URANUS_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    // ---- Uranus ----
    planets.emplace_back(
        ModelPaths::NEPTUNE_MODEL,
        glm::dvec3(Solar::NEPTUNE_AU, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, neptune_velocity),
        Solar::NEPTUNE_MASS,
        Solar::NEPTUNE_SIZE * SIZE_SCALE,
        glm::radians(20.0f)
    );

    Planet& sun   = planets[0];
    Planet& earth = planets[3];
    //Planet& moon  = planets[4];
    Planet& mars  = planets[5];



    // --------------------------------------------------
    // Texture Loading
    // --------------------------------------------------
    BackgroundTexture bgTex(Textures::STARS_TEX);
    
    float year {0.0f};
    ///////////////////////////////
    //      Main Render Loop
    //////////////////////////////
    window.setLastTime(glfwGetTime());
    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Timing 
        float currentTime = glfwGetTime();
        window.setDeltaTime(currentTime - window.getLastTime());
        window.setLastTime(currentTime);
        float angle = currentTime * glm::radians(50.0f); // 50 deg/sec

        // MVP
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // Our camera is -3 to start
        model = glm::rotate(model, currentTime * 0.5f, glm::vec3(0, 1, 0));

        glm::mat4 view = window.getCamera().GetViewMatrix();

        glm::mat4 projection = glm::perspective(
            glm::radians(window.getCamera().m_zoom),
            (float)Config::SCREEN_WIDTH / Config::SCREEN_HEIGHT,
            0.1f,
            1000.f
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

         // ------ Planets -------
        glEnable(GL_DEPTH_TEST);
        planetShader.Use();
        planetShader.SetMat4("view", view);
        planetShader.SetMat4("projection", projection);

        /*
        std::cout << planets[1].position.x << " "
          << planets[1].position.y << " "
          << planets[1].position.z << "\n";
        */

        //double d = glm::length(moon.position - earth.position);
        //std::cout << "Moon-Earth AU: " << d << "\n";

        //year += TIME_SCALE;
        //std::cout << year << "\n";

        for (int step = 0; step < PHYSICS_STEPS; ++step)
        {
            for (auto& p : planets)
                p.acceleration = glm::dvec3(0.0);

            for (size_t i = 0; i < planets.size(); ++i)
                for (size_t j = 0; j < planets.size(); ++j)
                    if (i != j)
                        planets[i].ApplyGravity(planets[j]);

            for (auto& p : planets)
                p.UpdatePhysics(PHYSICS_DT);

        }


        // rendering
        // rendering
        for (auto& p : planets) {
            p.UpdateRender(currentTime, DISTANCE_SCALE);
            p.Draw(planetShader);
        }
        

        // Extra
        window.processInput();

        window.swapBuffers();
        window.pollEvents();
    }
    
    
    
    return 0;
}



