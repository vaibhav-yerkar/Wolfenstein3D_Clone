# **Wolfenstein 3D Clone with SDL2 and OpenGL**

## **Overview**

This project is a recreation of the classic **Wolfenstein 3D** game built using **SDL2** and **OpenGL**. It features a raycasting engine to simulate 3D environments, dynamic text rendering for HUD elements, and smooth player movement and collision detection. The game combines SDL2 for input and window management with OpenGL for rendering.

---

## **Features**

- 🎮 2D Raycasting engine for 3D-like environments.
- 🌟 OpenGL-based rendering for enhanced graphics performance.
- 🧹 Clean and reusable SDL2 wrapper for better code management.
- 🕹️ Smooth player movement and collision detection.
- 🎶 Audio support with SDL_mixer for sound effects and music.

---

## GamePlay Screenshot

![GamePlay ScreenShot](./res/gameplay.png)

---

## **Setup and Installation**

### **Prerequisites**

- **C++ Compiler**: GCC, Clang, or MSVC with support for C++11 or newer.
- **SDL2**: For window management and input handling.
- **SDL_image**: For rendering images.
- **SDL_mixer**: For audio support (sound effects and music).
- **OpenGL**: For rendering graphics (requires OpenGL 4.1 or newer).
- **GLEW**: For managing OpenGL extensions.
- **GLM**: For OpenGL math-operations.

### **Installation Steps**

1. **Clone the repository**:

   ```bash
   git clone https://github.com/vaibhav-yerkar/Wolfenstein3D_Clone.git
   cd wolfenstein-3d-clone
   ```

2. **Install dependencies**:

   - **Linux (Debian-based)**:
     ```bash
     sudo apt install libsdl2-dev libsdl2_image-dev libsdl2_mixer-dev libglew-dev libglm-dev
     ```
   - **macOS**:
     ```bash
     brew install sdl2 sdl2_image glew glm sdl2_mixer
     ```
   - **Windows**:
     - Download SDL2 and SDL_image, SDL_mixer development libraries from their official websites.
     - Configure your IDE (e.g., Visual Studio) with the include and library paths.

3. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ../
   make
   ./wolfenstein3D
   ```

---

## **Controls**

- **W/A/S/D**: Move forward/left/backward/right.
- **E**: Door Eject.
- **Mouse Movement**: Rotate the player.
- **Mouse-left-buttom**: GunFire Attack.

---

## **Dependencies**

- **SDL2**: Window creation, input handling, and game loop management.
- **SDL_image**: image rendering .
- **SDL_mixer**: Audio support (sound effects and music).
- **OpenGL**: Rendering walls, sprites, and game objects.
- **GLEW**: OpenGL extension management.
- **C++ Standard Library**: Core utilities and STL.

---

## **Troubleshooting**

### **Common Issues**

1. **Blank Screen**:

   - Verify the OpenGL context is initialized.
   - Check if textures and shaders are loaded correctly.

2. **Compilation Errors**:
   - Ensure all dependencies (SDL2, SDL_ttf, OpenGL, GLEW, SDL_mixer) are installed.
   - Verify include and library paths for your compiler.

---

## **Future Improvements**

- 🔄 Improve collision detection for smoother gameplay.
- 💡 Add lighting effects and shading for realism.

---

## **Acknowledgments**

- **SDL2** and **OpenGL** communities for their excellent documentation and resources.
- The creators of **Wolfenstein 3D** for inspiring this project.

---

Feel free to update this file as necessary! 😊
