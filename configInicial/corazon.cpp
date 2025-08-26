#include <iostream>
#include <vector>
#include <cmath>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

// Elige modo: 0 = lineas, 1 = pixeles
#define MODE 1

// Ajustes de densidad
const int LINE_SAMPLES = 2000;     // mas alto = curva mas suave (modo lineas)
const int GRID_W = 160;            // mas alto = mas pixeles (modo pixeles)
const int GRID_H = 140;
const float POINT_SIZE = 6.0f;     // tamano visual del “pixel” (modo pixeles)

int main() {
    // Init
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 - Siiano Haller Rodrigo", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Geometria
    std::vector<float> vertices; // x,y,z,r,g,b

    auto pushV = [&](float x, float y, float r, float g, float b) {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b);
        };

#if MODE == 0
    // =================== MODO LINEAS CONTINUAS ===================
    // Curva paramétrica clasica:
    // x(t) = 16 sin^3 t
    // y(t) = 13 cos t - 5 cos 2t - 2 cos 3t - cos 4t
    // t en [0, 2?]. Escalamos para caber en NDC.
    vertices.reserve(6 * (LINE_SAMPLES + 1));
    const float scale = 0.05f; // ajusta el tamaño en pantalla
    for (int i = 0; i <= LINE_SAMPLES; ++i) {
        float t = (float)i / (float)LINE_SAMPLES * 6.28318530718f; // 2?
        float s = sinf(t), c = cosf(t);
        float x = 16.0f * s * s * s;
        float y = 13.0f * c - 5.0f * cosf(2 * t) - 2.0f * cosf(3 * t) - cosf(4 * t);
        x *= scale;
        y *= scale;
        pushV(x, y, 1.0f, 0.1f, 0.2f); // rojo-rosado
    }
#else
    // =================== MODO PIXELES (RELLENO) ===================
    // Ecuación implicita: (x^2 + y^2 - 1)^3 - x^2 y^3 <= 0
    // Muestreamos una rejilla centrada y pintamos los puntos “dentro”.
    vertices.reserve(6 * GRID_W * GRID_H);
    const float xMin = -1.4f, xMax = 1.4f;
    const float yMin = -1.3f, yMax = 1.3f;
    for (int j = 0; j < GRID_H; ++j) {
        for (int i = 0; i < GRID_W; ++i) {
            float fx = xMin + (xMax - xMin) * ((i + 0.5f) / GRID_W);
            float fy = yMin + (yMax - yMin) * ((j + 0.5f) / GRID_H);
            float r2 = fx * fx + fy * fy;
            float f = powf(r2 - 1.0f, 3.0f) - fx * fx * fy * fy * fy;
            if (f <= 0.0f) {
                // Escala ligera para dejar margen en la ventana
                pushV(fx * 0.9f, fy * 0.9f, 1.0f, 0.0f, 0.0f); // rojo puro
            }
        }
    }
#endif

    // ====== Subimos a GPU ======
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Atributos: pos (0), color (1)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ====== Render loop ======
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);

#if MODE == 0
        glLineWidth(3.0f);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 6));
#else
        glPointSize(POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size() / 6));
#endif

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
