# 🎯 Ray Tracer in C++ with SFML

A CPU-based ray tracer built from scratch using C++ and SFML 3.0. Features real-time rendering with reflections, shadows, and animated spheres.

![Ray Tracer Preview](preview.png)

## ✨ Features

- **3 Spheres** — Red, Blue, and Animated Metallic sphere
- **Reflections** — Mirror-like reflective surfaces
- **Shadows** — Soft shadow casting from multiple light sources
- **Specular Highlights** — Realistic light shading
- **Reflective Floor Plane** — Ground with reflection effect
- **2 Colored Lights** — Warm and cool light sources
- **Animated Rendering** — Metallic sphere bounces in real-time
- **Gradient Sky** — Blue sky background

## 🛠️ Built With

- **C++17**
- **SFML 3.0.2** — Window and graphics rendering
- **CMake** — Build system
- **MinGW (GCC 16.1)** — Compiler

## 📁 Project Structure

```
ray-tracer-cpp/
├── src/
│   └── main.cpp       # Full ray tracer source code
├── CMakeLists.txt     # CMake build configuration
├── .gitignore
└── README.md
```

## 🚀 How to Build

### Prerequisites
- [MinGW-w64](https://www.mingw-w64.org/)
- [CMake](https://cmake.org/)
- [vcpkg](https://github.com/microsoft/vcpkg) with SFML installed

### Install SFML via vcpkg
```bash
vcpkg install sfml:x86-mingw-dynamic
```

### Build Steps
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake \
         -DSFML_DIR=C:/vcpkg/installed/x86-mingw-dynamic/share/sfml \
         -DCMAKE_C_COMPILER="C:/mingw64/mingw32/bin/gcc.exe" \
         -DCMAKE_CXX_COMPILER="C:/mingw64/mingw32/bin/g++.exe" \
         -G "MinGW Makefiles"
mingw32-make
```

### Run
```bash
.\RayTracer.exe
```

> **Note:** Copy SFML debug DLLs to build folder:
> ```bash
> copy "C:\vcpkg\installed\x86-mingw-dynamic\debug\bin\*.dll" ".\build\"
> ```

## 🎮 Controls

| Key | Action |
|-----|--------|
| `R` | Re-render scene |
| `Esc` | Close window |

## 📐 Ray Tracing Concepts Used

- **Ray-Sphere Intersection** — Quadratic equation solving
- **Ray-Plane Intersection** — Dot product based
- **Phong Shading** — Ambient + Diffuse + Specular
- **Shadow Rays** — Secondary ray casting for shadows
- **Reflection Rays** — Recursive ray tracing (max depth 3)
- **Vec3 Math** — Custom 3D vector struct

## 👤 Author

**Snehal Thombare**  
GitHub: [@snehal-thombare08](https://github.com/snehal-thombare08)
