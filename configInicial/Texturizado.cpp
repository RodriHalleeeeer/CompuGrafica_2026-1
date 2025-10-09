
//Siliano Haller Rodrigo
//No.Cuenta: 319039627
//Practica 7 
//Fecha de entrega: 10/10/2025



// main.cpp
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Otros
#include "Shader.h"
#include "Camera.h"

// Prototipos
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Dimensiones de ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Cámara
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024] = { false };
bool firstMouse = true;

// Tiempo
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 7 - Siliano Haller Rodrigo", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Init GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cerr << "Failed to init GLEW\n";
        return EXIT_FAILURE;
    }

    // Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL state
    glEnable(GL_DEPTH_TEST);
   
    // Shader
    Shader shader("Shader/lamp.vs", "Shader/lamp.frag");

  
    GLfloat vertices[] = {
        // ===== BACK (num 6)  cell (1,1)  U:[0.3333333,0.6666667]  V:[0.25,0.50]  normal (0,0,-1)
        -0.5f,-0.5f,-0.5f,   0,0,-1,   0.3333333f, 0.25f,
         0.5f, 0.5f,-0.5f,   0,0,-1,   0.6666667f, 0.50f,
         0.5f,-0.5f,-0.5f,   0,0,-1,   0.6666667f, 0.25f,

         0.5f, 0.5f,-0.5f,   0,0,-1,   0.6666667f, 0.50f,
        -0.5f,-0.5f,-0.5f,   0,0,-1,   0.3333333f, 0.25f,
        -0.5f, 0.5f,-0.5f,   0,0,-1,   0.3333333f, 0.50f,

        // ===== FRONT (num 4) cell (1,0)  U:[0.3333333,0.6666667]  V:[0.00,0.25]  normal (0,0,1)
        -0.5f,-0.5f, 0.5f,   0,0, 1,   0.3333333f, 0.00f,
         0.5f,-0.5f, 0.5f,   0,0, 1,   0.6666667f, 0.00f,
         0.5f, 0.5f, 0.5f,   0,0, 1,   0.6666667f, 0.25f,

         0.5f, 0.5f, 0.5f,   0,0, 1,   0.6666667f, 0.25f,
        -0.5f, 0.5f, 0.5f,   0,0, 1,   0.3333333f, 0.25f,
        -0.5f,-0.5f, 0.5f,   0,0, 1,   0.3333333f, 0.00f,

        // --- LEFT FACE (num 2) cell (2,1)  normal (-1,0,0)
-0.5f, 0.5f, 0.5f,  -1,0,0,   1.0000000f, 0.50f,
-0.5f, 0.5f,-0.5f,  -1,0,0,   0.6666667f, 0.50f,
-0.5f,-0.5f,-0.5f,  -1,0,0,   0.6666667f, 0.25f,

-0.5f,-0.5f,-0.5f,  -1,0,0,   0.6666667f, 0.25f,
-0.5f,-0.5f, 0.5f,  -1,0,0,   1.0000000f, 0.25f,
-0.5f, 0.5f, 0.5f,  -1,0,0,   1.0000000f, 0.50f,



  // --- RIGHT FACE (2) ---
   0.5f, 0.5f, 0.5f,   1,0,0,   1.0000000f, 0.50f,
   0.5f,-0.5f,-0.5f,   1,0,0,   0.6666667f, 0.25f,
   0.5f, 0.5f,-0.5f,   1,0,0,   0.6666667f, 0.50f,

   0.5f,-0.5f,-0.5f,   1,0,0,   0.6666667f, 0.25f,
   0.5f, 0.5f, 0.5f,   1,0,0,   1.0000000f, 0.50f,
   0.5f,-0.5f, 0.5f,   1,0,0,   1.0000000f, 0.25f,



         // ===== TOP   (num 3) cell (1,2)  U:[0.3333333,0.6666667]  V:[0.50,0.75]  normal (0,1,0)
         -0.5f, 0.5f,-0.5f,   0,1, 0,   0.3333333f, 0.50f,
          0.5f, 0.5f, 0.5f,   0,1, 0,   0.6666667f, 0.75f,
          0.5f, 0.5f,-0.5f,   0,1, 0,   0.6666667f, 0.50f,

          0.5f, 0.5f, 0.5f,   0,1, 0,   0.6666667f, 0.75f,
         -0.5f, 0.5f,-0.5f,   0,1, 0,   0.3333333f, 0.50f,
         -0.5f, 0.5f, 0.5f,   0,1, 0,   0.3333333f, 0.75f,

         // ===== BOTTOM (num 1) cell (1,3)  U:[0.3333333,0.6666667]  V:[0.75,1.00]  normal (0,-1,0)
         -0.5f,-0.5f,-0.5f,   0,-1,0,   0.3333333f, 1.00f,
          0.5f,-0.5f,-0.5f,   0,-1,0,   0.6666667f, 1.00f,
          0.5f,-0.5f, 0.5f,   0,-1,0,   0.6666667f, 0.75f,

          0.5f,-0.5f, 0.5f,   0,-1,0,   0.6666667f, 0.75f,
         -0.5f,-0.5f, 0.5f,   0,-1,0,   0.3333333f, 0.75f,
         -0.5f,-0.5f,-0.5f,   0,-1,0,   0.3333333f, 1.00f
    };





   
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributos: pos(0), color(1), uv(2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Textura
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);



    // Filtros/parámetros (MAG no admite mipmaps)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // CLAMP_TO_EDGE si hay halos con alpha
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int texW, texH, nCh;
    unsigned char* data = stbi_load("images/dado.png", &texW, &texH, &nCh, 0);
    if (!data) {
        std::cerr << "Failed to load texture\n";
    }
    else {
        // Para imágenes con ancho no múltiplo de 4 puede ser útil:
        // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum fmt = (nCh == 1) ? GL_RED : (nCh == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, texW, texH, 0, fmt, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    // Sampler del shader a la unidad 0
    shader.Use();
    glUniform1i(glGetUniformLocation(shader.Program, "diffuse"), 0);

    // Bucle principal
    while (!glfwWindowShouldClose(window))
    {
        // Delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        glfwPollEvents();
        DoMovement();

        // Clear
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        // Matrices de cámara
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()),
            (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Modelo (rotación para apreciar el 3D)
        //float t = glfwGetTime();
        glm::mat4 model(1.0f);
      //  model = glm::rotate(model, t * glm::radians(35.0f), glm::vec3(0.5f, 1.0f, 0.2f));

        // Uniforms
        GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint projLoc = glGetUniformLocation(shader.Program, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind de textura y draw
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // Limpieza
    glDeleteTextures(1, &texture1);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// Movimiento con teclado
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

// Mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
        lastX = (float)xPos;
        lastY = (float)yPos;
        firstMouse = false;
    }
    GLfloat xOffset = (float)xPos - lastX;
    GLfloat yOffset = lastY - (float)yPos; // invertido: pantalla va de top->bottom
    lastX = (float)xPos;
    lastY = (float)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
