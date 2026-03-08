// GLFW and GLAD
#include <glad.h>
#include <GLFW/glfw3.h>

// STL
#include <iostream>
#include <array>

// Own headers
#include <shaders/Shader.h>
#include <renderer/VertexAttribute.h>
#include <renderer/IndexBuffer.h>
#include <transform/Transform.hpp>
#include <transform/Camera.hpp>
#include <mini_audio_engine/Audio.h>

// Third-party libraries
#include <third-party/stb_image.h>

// GLM
#include <third-party/glm/glm.hpp>
#include <third-party/glm/gtc/matrix_transform.hpp>
#include <third-party/glm/gtc/type_ptr.hpp>

namespace {
    const char* CAPTION = "Homework #3";

    constexpr int   SCR_WIDTH = 800;
    constexpr int   SCR_HEIGHT = 800;
    constexpr float MIX_DURATION = 0.5f; // seconds for full transition

    // Mixing textures variables
    float g_mixValue = 0.0f; // 0 -> only first image, 1 -> only second
    bool  g_animating = false;
    float g_direction = 1.0f; // +1: first -> second, -1: second -> first

    // Audio variables
    Audio g_xqcHappy; // xqc is the name of the youtuber in meme (I guess)
    Audio g_xqcSad;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    constexpr std::array vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,   // bottom-left
         0.5f, -0.5f,  0.5f,   // bottom-right
         0.5f,  0.5f,  0.5f,   // top-right
        -0.5f,  0.5f,  0.5f,   // top-left

        // Back face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Left face
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Right face
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

         // Top face
         -0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f, -0.5f,
         -0.5f,  0.5f, -0.5f,

         // Bottom face
         -0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f
    };


    constexpr std::array indices = {
    0, 1, 2,  2, 3, 0,        // Front
    4, 5, 6,  6, 7, 4,        // Back
    8, 9,10, 10,11, 8,        // Left
    12,13,14, 14,15,12,       // Right
    16,17,18, 18,19,16,       // Top
    20,21,22, 22,23,20        // Bottom
    };


    constexpr std::array textureCoords = {
        // Front
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Back
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Left
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Right
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Top
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };


    void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    void handle_click(GLFWwindow* window)
    {
        static bool lastLeftState = false;
        const bool currentLeftState =
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        if (currentLeftState && !lastLeftState)
        {
            g_animating = true;
            g_direction = (g_mixValue < 0.5f) ? +1.0f : -1.0f;

            g_xqcHappy.Stop();
            g_xqcSad.Stop();

            if (g_direction > 0.0f) {
                // state: happy -> sad
                g_xqcSad.Rewind(); 
                g_xqcSad.Play(true);
            }
            else {
                // state: sad -> happy
				g_xqcHappy.Rewind(); 
                g_xqcHappy.Play(true);
            }
        }

        lastLeftState = currentLeftState;
    }

    void processInput(GLFWwindow* window, Transform& t, float deltaTime)
    {
	    constexpr float moveSpeed = 2.0f;
	    constexpr float scaleSpeed = 1.0f;       

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

    	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	    {
		    constexpr float rotSpeed = 90.0f;
		    t.Rotate({ rotSpeed * deltaTime, rotSpeed * deltaTime, rotSpeed * deltaTime });
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            t.Move({ 0.0f, +moveSpeed * deltaTime, 0.0f });
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            t.Move({ 0.0f, -moveSpeed * deltaTime, 0.0f });
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            t.Move({ -moveSpeed * deltaTime, 0.0f, 0.0f });
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            t.Move({ +moveSpeed * deltaTime, 0.0f, 0.0f });
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            float factor = 1.0f + scaleSpeed * deltaTime;
            t.Scale({ factor, factor, factor });   
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            float factor = 1.0f - scaleSpeed * deltaTime;
            t.Scale({ factor, factor, factor });  
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            t = Transform{}; 
        }
    }


    GLenum DetectTextureFormat(const int numColCh)
    {
        switch (numColCh)
        {
        case 1: return GL_RED;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default:
            std::cout << "Unsupported number of color channels: " << numColCh << '\n';
            return GL_RGBA; // fallback
        }
    }

    unsigned int LoadTexture2D(const char* path,
        GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
        GLint magFilter = GL_LINEAR)
    {
        int width{}, height{}, numColCh{};
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(path, &width, &height, &numColCh, 0);
        if (!data)
        {
            std::cerr << "Failed to load texture: " << path << '\n';
            return 0;
        }

        const GLenum format = DetectTextureFormat(numColCh);

        unsigned int texture{};
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        glTexImage2D(GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        return texture;
    }

} // anonymous namespace

int main()
{
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window and make context for OpenGL resources
    const auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, CAPTION, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Viewport: tells where inside the window to draw and how to map normalized device coordinates (NDC) to pixels
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initializing GLAD. Without it, we can't get the addresses of OpenGL functions from the graphics driver.
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
        std::cerr << "Failed to initialize GLAD" << '\n';
        glfwTerminate();
        return -1;
    }

    // Initializing Audio
    if (!Audio::Init()) {
        glfwTerminate();
        return -1;
    }

    if (!g_xqcHappy.Load("audio/xqc_happy.mp3")) {
        std::cerr << "Failed to load xqc_happy.mp3\n";
    }

    if (!g_xqcSad.Load("audio/xqc_sad.mp3")) {
        std::cerr << "Failed to load xqc_sad.mp3\n";
    }

    g_xqcHappy.Play();   // 🎵 play happy sound on startup

    // Build and compile our shader program
    Shader ourShader{
        "shaders/GLSL/triangle_vertex.vert",
        "shaders/GLSL/triangle_fragment.frag"
    };

    // VBO, VAO and other important stuff for a graphics pipeline
    VertexAttributeObject VAO{};
    VAO.Bind();

    VertexBufferObject VBO{ vertices.data(), static_cast<GLsizeiptr>(vertices.size() * sizeof(float)) };
    IndexBufferObject  EBO{ indices.data(),  static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)) };

    // VBO for texture coordinates
    VertexBufferObject TBO{ textureCoords.data(), static_cast<GLsizeiptr>(textureCoords.size() * sizeof(float)) };

    // Position: aPos (location = 0)
    VAO.LinkVBO(VBO, 3, 3 * sizeof(float), 0);

    // Texture Coords: aTexCoord (location = 1)
    VAO.LinkVBO(TBO, 2, 2 * sizeof(float), 1);

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
    TBO.Unbind();

    // Textures
    const unsigned int texture = LoadTexture2D("images/verbatim_01.png",
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR);
    if (texture == 0) {
        glfwTerminate();
        Audio::Shutdown();
        return -1;
    }

    const unsigned int texture2 = LoadTexture2D("images/verbatim_02.png",
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR);
    if (texture2 == 0) {
        glfwTerminate();
        Audio::Shutdown();
        return -1;
    }

    ourShader.Use();
    ourShader.SetInt("uTexture", 0);
    ourShader.SetInt("uTexture2", 1);

    Transform modelTransform{};

    float lastTime = glfwGetTime();
    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
	    Camera camera{};
	    const float currentTime = glfwGetTime();
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // input
        processInput(window, modelTransform, deltaTime);
        handle_click(window);

        if (g_animating) {
            g_mixValue += g_direction * (deltaTime / MIX_DURATION);

            if (g_mixValue <= 0.0f) {
                g_mixValue = 0.0f;
                g_animating = false;
            }
            else if (g_mixValue >= 1.0f) {
                g_mixValue = 1.0f;
                g_animating = false;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw whatever we draw
        ourShader.Use();
        ourShader.SetFloat("mixValue", g_mixValue);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        const glm::mat4 model = modelTransform.GetModelMatrix();
		const glm::mat4 view = camera.GetViewMatrix();
		const glm::mat4 projection = camera.GetProjectionMatrix();

		const auto mvp = projection * view * model;

        const GLint mvpLoc = glGetUniformLocation(ourShader.GetID(), "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

        // swap buffers and poll IO events (keys pressed/released, mouse moved, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional cleanup
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    TBO.Delete();

    glfwTerminate();
    Audio::Shutdown();

    return 0;
}
