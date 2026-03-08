# OpenGL Cube Demo

Small C++/OpenGL learning project built as a Visual Studio solution. The application opens a GLFW window, renders a textured 3D cube, blends between two images on mouse click, and swaps short audio clips to match the visual state.

## Overview

This repository contains one native Windows desktop project:

- `ConsoleApplication1.slnx`: solution file
- `ConsoleApplication1/ConsoleApplication1.vcxproj`: C++ application project

At runtime the app:

- creates an OpenGL 3.3 core context with GLFW
- loads OpenGL functions with GLAD
- renders a cube using indexed drawing
- applies two textures and interpolates between them with a shader uniform
- plays MP3 audio through `miniaudio`
- lets the user move, scale, and rotate the cube with the keyboard

## Features

- Textured cube rendered with vertex/index buffers and a VAO wrapper
- Shader abstraction for loading, compiling, linking, and setting uniforms
- `Transform` and `Camera` helpers using GLM
- Texture loading with `stb_image`
- Simple audio wrapper around `miniaudio`
- Input-driven state transition:
  - left mouse click blends between `verbatim_01.png` and `verbatim_02.png`
  - the matching `xqc_happy.mp3` / `xqc_sad.mp3` clip plays during the transition

## Controls

- `Esc`: close the application
- `Space`: rotate the cube continuously while held
- `W` / `A` / `S` / `D`: move the cube on the X/Y plane
- `Q`: scale up
- `E`: scale down
- `R`: reset transform
- Left mouse button: toggle texture blend direction and play the corresponding sound

## Project Structure

```text
OpenGL/
|- ConsoleApplication1.slnx
|- ConsoleApplication1/
|  |- audio/                     # MP3 assets
|  |- images/                    # Texture assets
|  |- include/
|  |  |- mini_audio_engine/      # Audio wrapper
|  |  |- renderer/               # VBO / EBO / VAO wrappers
|  |  |- transform/              # Transform and camera helpers
|  |  |- third-party/            # Bundled GLM, stb_image, miniaudio
|  |- shaders/
|  |  |- GLSL/                   # Vertex/fragment shaders
|  |  |- Shader.h
|  |- src/
|  |  |- main.cpp                # Application entry point
|  |  |- mini_audio_engine/
|  |  |- renderer/
|  |  |- shaders/
|  |  |- third-party/            # glad.cpp and stb implementation unit
|- LICENSE.txt
```

## Technologies

- C++20
- OpenGL
- GLFW
- GLAD
- GLM
- stb_image
- miniaudio
- Visual Studio / MSVC project system

## Build Requirements

The repository is not fully self-contained. You need external GLFW and GLAD headers/libraries available on your machine.

Required environment:

- Windows
- Visual Studio with C++ desktop tooling
- An MSVC toolset compatible with `v145` or a retargeted project
- GLFW headers and library
- GLAD headers
- OpenGL driver support (OpenGL 3.3+)

Bundled in the repo:

- GLM headers
- `stb_image`
- `miniaudio`
- generated `glad.cpp`

Not bundled in the repo:

- GLFW binaries/lib files
- GLAD header files

## Important Setup Notes

The x64 Debug configuration in `ConsoleApplication1.vcxproj` uses these placeholders:

- `$(GLFW_INCLUDE_DIR)`
- `$(GLAD_INCLUDE_DIR)`
- `$(GLFW_LIB_DIR)`

You must provide these values either:

- as system/user environment variables, or
- by replacing them in project properties (`AdditionalIncludeDirectories` / `AdditionalLibraryDirectories`)

Also ensure `glfw3.dll` is available at runtime (next to the `.exe` or in `PATH`) if you link dynamically.

## Build And Run (Visual Studio)

1. Open `ConsoleApplication1.slnx` in Visual Studio.
2. Retarget the project if Visual Studio prompts for a newer Windows SDK or toolset.
3. Configure `GLFW_INCLUDE_DIR`, `GLAD_INCLUDE_DIR`, and `GLFW_LIB_DIR`.
4. Build the `x64 Debug` configuration.
5. Set working directory to `$(ProjectDir)` in Debugging settings.
6. Start the app (`F5` or `Ctrl+F5`).

The working directory must resolve asset paths used by the app:

- `audio/xqc_happy.mp3`
- `audio/xqc_sad.mp3`
- `images/verbatim_01.png`
- `images/verbatim_02.png`
- `shaders/GLSL/triangle_vertex.vert`
- `shaders/GLSL/triangle_fragment.frag`

## Optional CLI Build (Developer Command Prompt)

If you have MSBuild configured:

```powershell
msbuild ConsoleApplication1.slnx /p:Configuration=Debug /p:Platform=x64
```

Then run:

```powershell
cd ConsoleApplication1
..\\x64\\Debug\\ConsoleApplication1.exe
```

## How It Works

Core rendering path in `src/main.cpp`:

- initializes GLFW and creates an `800 x 800` window
- initializes GLAD after the OpenGL context is created
- initializes the global `Audio` engine
- loads two textures and two audio clips
- creates:
  - one VBO for cube positions
  - one VBO for texture coordinates
  - one EBO for indexed drawing
  - one VAO to bind vertex layout
- calculates an `MVP` matrix from:
  - `Transform` model matrix
  - fixed `Camera` view matrix
  - perspective projection
- updates a `mixValue` uniform over time to animate the texture transition
- draws the cube with `glDrawElements`

## Current Limitations

- The repo includes `glad.cpp` but not the GLAD headers, so a fresh checkout will not compile without additional setup.
- No automated tests are present.
- The projection matrix uses a fixed aspect ratio of `1.0f`; resizing the window updates the viewport but not the camera aspect ratio, so the image can stretch.
- Shader inputs/varyings include an unused color attribute path that is not fully wired up.
- `LICENSE.txt` is the MIT template but still contains placeholders (`[year]`, `[fullname]`).

## Suggested Cleanup

- Vendor GLFW and missing GLAD headers into the repo, or switch to a package manager.
- Keep dependency paths portable (env vars or relative paths) in `.vcxproj`.
- Fix the unused color attribute/varying path in the shaders.
- Update the camera projection aspect ratio from the current framebuffer size.
- Replace the placeholder values in `LICENSE.txt`.
- Add screenshots or a short GIF once the project is stable.

## License

`LICENSE.txt` contains the MIT license template, but the copyright placeholders have not been filled in yet.
