//Siliano Haller Rodrigo
//Numero de cuenta: 319039627
//Practica 2 
//Fecha de entrega: 21/08/2025



#include <iostream>
#include <vector>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 - Siiano Haller Rodrigo", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);
    if (window == NULL) { std::cout << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) { std::cout << "Failed to initialise GLEW\n"; return EXIT_FAILURE; }

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // ====== PATRON PIXEL-ART (15x12). '1' = bloque del corazon ======
    const int gridW = 15, gridH = 12;
    const char* heart[gridH] = {
        "....11...11....",
        "...1111.1111...",
        "..11111111111..",
        ".1111111111111.",
        ".1111111111111.",
        ".1111111111111.",
        "..11111111111..",
        "...111111111...",
        "....1111111....",
        ".....11111.....",
        "......111......",
        ".......1......."
    };

    auto isFilled = [&](int x, int y) -> bool {
        if (x < 0 || x >= gridW || y < 0 || y >= gridH) return false;
        return heart[y][x] == '1';
        };

    // ====== Mapeo a NDC: cuadrado, centrado y con margen correcto ======
    const float margin = 0.10f; // 10% de margen alrededor
    const float cell = 2.0f * (1.0f - margin) / float(std::max(gridW, gridH)); // <<< fijo
    const float totalW = gridW * cell;
    const float totalH = gridH * cell;
    const float originX = -totalW * 0.5f;
    const float originY = -totalH * 0.5f;

    std::vector<float> vertices; // x,y,z,r,g,b
    vertices.reserve(6 * gridW * gridH * 8); // aprox

    auto addLine = [&](float x0, float y0, float x1, float y1) {
        vertices.insert(vertices.end(), {
            x0, y0, 0.0f, 1.0f, 0.0f, 0.0f,
            x1, y1, 0.0f, 1.0f, 0.0f, 0.0f
            });
        };

    // ====== Generacion de contorno ======
    for (int gy = 0; gy < gridH; ++gy) {
        for (int gx = 0; gx < gridW; ++gx) {
            if (!isFilled(gx, gy)) continue;

            // Invertimos Y para que heart[0] sea ARRIBA en pantalla
            float x0 = originX + gx * cell;
            float x1 = x0 + cell;
            float y0 = originY + (gridH - 1 - gy) * cell; // <<< fijo
            float y1 = y0 + cell;

            // Borde IZQUIERDO
            if (!isFilled(gx - 1, gy)) addLine(x0, y0, x0, y1);
            // Borde DERECHO
            if (!isFilled(gx + 1, gy)) addLine(x1, y0, x1, y1);
            // Borde INFERIOR (abajo en pantalla corresponde a gy+1 en la mascara)  <<< fijo
            if (!isFilled(gx, gy + 1)) addLine(x0, y0, x1, y0);
            // Borde SUPERIOR (arriba en pantalla corresponde a gy-1 en la mascara) <<< fijo
            if (!isFilled(gx, gy - 1)) addLine(x0, y1, x1, y1);
        }
    }

    // ====== Subir a GPU ======
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ====== Render ======
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);
        glLineWidth(5.0f);              // grosor estilo “bloques”
        glDrawArrays(GL_LINES, 0, (GLsizei)(vertices.size() / 6));
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
