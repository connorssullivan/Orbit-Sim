#include "planet.h"


static constexpr float G = 39.47841760435743; // gravitational constant

Planet::Planet(const char* modelPath,
               glm::vec3 startPos,
               glm::vec3 startVel,
               float mass,
               float scale,
               float spinSpeed)
    : model(modelPath),
      position(startPos),
      velocity(startVel),
      mass(mass),
      scale(scale),
      spinSpeed(spinSpeed)
{}


void Planet::ApplyGravity(const Planet& other)
{
    glm::dvec3 r = other.position - position;
    double dist = glm::length(r);

    // avoid singularity
    dist = glm::max(dist, 1e-4);

    glm::vec3 dir = r / dist;

    float accel = G * other.mass / (dist * dist);
    acceleration += dir * accel;
}


void Planet::UpdatePhysics(double dt)
{
    // Velocity Verlet integration

    // 1. Update position using current velocity and acceleration
    position += velocity * dt
              + 0.5 * acceleration * dt * dt;

    // 2. Update velocity using average acceleration
    velocity += 0.5 * (previousAcceleration + acceleration) * dt;

    // 3. Store acceleration for next frame
    previousAcceleration = acceleration;

    // 4. Reset acceleration accumulator
    acceleration = glm::vec3(0.0f);
}



void Planet::UpdateRender(float time, double distanceScale)
{
    modelMatrix = glm::mat4(1.0f);

    glm::dvec3 renderPos = position * distanceScale;

    modelMatrix = glm::translate(
        modelMatrix,
        glm::vec3(renderPos)
    );

    if (spinSpeed != 0.0f)
        modelMatrix = glm::rotate(
            modelMatrix,
            time * spinSpeed,
            glm::vec3(0, 1, 0)
        );

    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
}


void Planet::Draw(Shader& shader)
{
    shader.Use();
    shader.SetMat4("model", modelMatrix);
    model.Draw(shader);
}

