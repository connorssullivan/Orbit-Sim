# 🌌 OrbitSim — Modern OpenGL Solar System (C++)

A modern OpenGL-based solar system simulation written in C++. The project focuses on clean engine architecture, explicit OpenGL state management, and real-time orbital mechanics using physically motivated units.

&#x20;  &#x20;

---

## 🪐 Overview

**OrbitSim** is a real-time OpenGL application that visualizes a simplified 3D solar system using Newtonian gravity. It is designed as a portfolio-quality project demonstrating modern C++ practices, graphics programming fundamentals, and clean system separation inspired by LearnOpenGL-style engines.

---

## ✨ Features

- **Real-Time Orbital Mechanics**
  - Newtonian gravity using astronomical units
  - Stable orbits with adjustable timestep
- **Modern OpenGL Rendering**
  - Core profile (3.3+)
  - VAO / VBO / EBO abstractions
  - Fully shader-based pipeline
- **Model Loading**
  - Assimp-powered `.obj` loading
  - Multiple textures per mesh
- **Camera System**
  - Free-look camera (yaw, pitch, zoom)
  - Perspective projection
- **Texture System**
  - Diffuse and specular maps
  - stb\_image integration
- **Clean Architecture**
  - Renderer abstraction
  - RAII-managed OpenGL objects
  - No hidden global state
- **Cross-Platform**
  - macOS, Linux, Windows
  - CMake-based builds with submodules

---

## 🏗️ Architecture

### Core Systems

- **Window**
  - GLFW initialization and context management
  - Input polling and lifecycle control
- **Renderer**
  - Centralized draw submission
  - Clear separation of CPU/GPU responsibilities
- **Shader**
  - Uniform abstraction with type-safe setters
- **Buffers**
  - `VertexBuffer`, `IndexBuffer`, `VertexArray`
- **Model / Mesh**
  - Assimp scene parsing
  - Per-mesh texture binding
- **Camera**
  - View and projection matrix generation
- **Planet System**
  - Mass, velocity, and acceleration
  - Physics integration per frame

---

## 🧠 Design Principles

- RAII-first OpenGL resource management
- Explicit ownership and lifetime control
- No implicit global state
- Target-based, modern CMake
- Readable math and physics code

---

## 🛠️ Technical Stack

- **Language**: C++20
- **Graphics API**: OpenGL 3.3 Core
- **Windowing/Input**: GLFW
- **Loading**: GLAD, Assimp, stb\_image
- **Math**: GLM
- **Build System**: CMake
- **Platforms**: macOS / Linux / Windows

---

## 🚀 Getting Started

### Prerequisites

- C++20-compatible compiler (Clang, GCC, MSVC)
- CMake 3.10+
- Git

#### Linux dependencies (Debian/Ubuntu)

```bash
sudo apt install -y \
  libxrandr-dev libxcursor-dev libxi-dev libudev-dev \
  libgl1-mesa-dev
```

---

### Build Instructions

```bash
git clone --recurse-submodules https://github.com/yourname/OrbitSim.git
cd OrbitSim

cmake -S . -B build
cmake --build build -j
```

Run the application:

```bash
./build/main
```

---

## 🎮 Controls

- **W / A / S / D** — Move camera
- **Mouse** — Look around
- **Scroll Wheel** — Zoom
- **ESC** — Exit

---

## 📁 Project Structure

```text
OrbitSim/
├── assets/
│   ├── models/
│   └── textures/
├── include/
│   ├── core/        # Window, Renderer, Camera
│   ├── graphics/    # Shader, Buffers
│   ├── model/       # Model, Mesh
│   └── physics/     # Planet, Gravity
├── src/
│   ├── main.cpp
│   ├── renderer.cpp
│   ├── camera.cpp
│   ├── model.cpp
│   └── planet.cpp
├── glfw/            # GLFW submodule
├── assimp/          # Assimp submodule
└── CMakeLists.txt
```

---

## 🔬 Key Implementation Details

### Rendering Pipeline

- CPU-side scene updates
- Per-frame uniform updates
- Per-mesh draw calls with texture batching

### Physics

- Semi-implicit Euler integration
- Units normalized to AU / years / solar masses
- Stable timestep control

### Resource Management

- All OpenGL objects wrapped in RAII classes
- No raw `glDelete*` calls outside destructors

---

## 📊 Performance

- Designed for 60+ FPS
- Minimal per-frame allocations
- Efficient buffer reuse

---

## 📌 Roadmap

-

---

## 📝 License

This project is licensed under the MIT License — see `LICENSE` for details.

---

## 🙏 Acknowledgments

- LearnOpenGL
- GLFW, GLAD, Assimp communities
- Khronos Group

---

Built with modern C++ and OpenGL. Designed to demonstrate clean architecture, graphics fundamentals, and engine-style system design suitable for SWE and graphics-focused roles.

