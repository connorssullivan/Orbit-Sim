#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "shader.h"

class Planet {
private:
    Model model;

public:
    // ---- Physics ----
    glm::dvec3 position {0.0};
    glm::dvec3 velocity {0.0};
    glm::dvec3 acceleration {0.0};
    glm::dvec3 previousAcceleration {0.0};
    double mass;

    // ---- Visual ----
    float scale;
    float spinSpeed;
    glm::mat4 modelMatrix {1.0f};

    Planet(const char* modelPath,
           glm::vec3 startPos,
           glm::vec3 startVel,
           float mass,
           float scale = 1.0f,
           float spinSpeed = 0.0f);

    // Physics
    void ApplyGravity(const Planet& other);
    void UpdatePhysics(double dt);

    // Rendering
    void UpdateRender(float time, double distanceScale);
    void Draw(Shader& shader);
};