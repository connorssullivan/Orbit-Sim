#pragma once


namespace Config
{
    constexpr unsigned int SCREEN_WIDTH {800};
    constexpr unsigned int SCREEN_HEIGHT {600};
};

namespace Solar 
{
    // ---- Units ----
    // Distance: AU
    // Time: years
    // Mass: solar masses

    constexpr double G = 39.47841760435743; // 4π²

    // ---- Star ----
    constexpr double SUN_MASS = 1.0;

    // ---- Planets (solar masses) ----
    constexpr double MERCURY_MASS = 1.651e-7;
    constexpr double VENUS_MASS   = 2.447e-6;
    constexpr double EARTH_MASS   = 3.003e-6;
    // ---- Moon (solar masses) ----
    // Moon mass ≈ 7.342e22 kg; Sun mass ≈ 1.9885e30 kg → 3.694e-8 solar masses
    constexpr double MOON_MASS    = 3.694e-8;
    constexpr double MARS_MASS    = 3.213e-7;
    constexpr double JUPITER_MASS = 9.545e-4;
    constexpr double SATURN_MASS  = 2.858e-4;
    constexpr double URANUS_MASS  = 4.366e-5;
    constexpr double NEPTUNE_MASS = 5.151e-5;

    // ---- Semi-major axes (AU) ----
    constexpr double MERCURY_AU = 0.387;
    constexpr double VENUS_AU   = 0.723;
    constexpr double EARTH_AU   = 1.000;
    constexpr double MARS_AU    = 1.524;
    constexpr double JUPITER_AU = 5.204;
    constexpr double SATURN_AU  = 9.582;
    constexpr double URANUS_AU  = 19.201;
    constexpr double NEPTUNE_AU = 30.047;

    // ---- Moon orbital radius about Earth (AU) ----
    // Mean Earth–Moon distance ≈ 384,400 km; 1 AU ≈ 149,597,870.7 km → 0.002569 AU
    constexpr double MOON_EARTH_AU = 0.002569;

    // ---- Distances ----
    constexpr double SUN_DISTANCE     = 0.0;

    constexpr double MERCURY_DISTANCE = 4.16;
    constexpr double VENUS_DISTANCE   = 7.77;
    constexpr double EARTH_DISTANCE   = 10.75; // * 10 for actual distance
    constexpr double MARS_DISTANCE    = 16.385;
    constexpr double JUPITER_DISTANCE = 55.93;
    constexpr double SATURN_DISTANCE  = 103.085;
    constexpr double URANUS_DISTANCE  = 206.48;
    constexpr double NEPTUNE_DISTANCE = 323.095;


    // ---- Sizes (Relative to sun of 1.0) ----
    constexpr double MERCURY_SIZE  = 0.0035;
    constexpr double VENUS_SIZE    = 0.0087;
    constexpr double EARTH_SIZE    = 0.0092;
    // Moon radius ≈ 1737.4 km; Sun radius ≈ 696,340 km → 0.00249
    constexpr double MOON_SIZE     = 0.00249;
    constexpr double MARS_SIZE     = 0.0049;
    constexpr double JUPITER_SIZE  = 0.100;
    constexpr double SATURN_SIZE   = 0.084;
    constexpr double URANUS_SIZE   = 0.036;
    constexpr double NEPTUNE_SIZE  = 0.035;

    // ===============================
    // Orbital speeds (AU / year)
    // v = sqrt(G * M_sun / r)
    // ===============================
    inline double orbitalSpeed(double rAU)
    {
        return std::sqrt(G * SUN_MASS / rAU);
    }

    // Circular orbit speed around an arbitrary central mass (solar masses), at radius rAU (AU)
    inline double orbitalSpeedAround(double centralMass, double rAU)
    {
        return std::sqrt(G * centralMass / rAU);
    }

    // Convenience: circular Moon orbit speed around Earth at mean distance (AU/year)
    inline double moonOrbitalSpeed()
    {
        return orbitalSpeedAround(EARTH_MASS + MOON_MASS, MOON_EARTH_AU);
    }
};

// You can do this later
/*
inline const BodyInit Saturn {
        "Saturn",
        2.858e-4,
        glm::dvec3(9.582, 0.0, 0.0),
        glm::dvec3(0.0, 0.0, circularSpeed(9.582)),
        0.5
    };

*/