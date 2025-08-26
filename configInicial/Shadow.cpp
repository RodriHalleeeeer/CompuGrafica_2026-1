#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);
const GLint WIDTH = 800, HEIGHT = 600;

// Mostrar contorno tipo “pixel”
#define DRAW_CONTOUR 1

struct RGB { float r, g, b; };
RGB colorOf(char c) {
    switch (c) {
    case 'K': return { 0.05f,0.05f,0.05f }; // negro
    case 'R': return { 1.00f,0.10f,0.10f }; // rojo
    case 'P': return { 1.00f,0.82f,0.60f }; // piel
    case 'W': return { 1.00f,1.00f,1.00f }; // blanco
    case 'Y': return { 1.00f,0.85f,0.10f }; // amarillo (aros)
    default:  return { 0.0f,0.0f,0.0f };
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shadow Pixel Art (Full Body)", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);
    if (!window) { std::cout << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) { std::cout << "Failed to initialise GLEW\n"; return EXIT_FAILURE; }

    Shader shader("Shader/core.vs", "Shader/core.frag");

    // =================== SPRITE 16x18 (cuerpo completo) ===================
    std::vector<std::string> sprite = {
    "....RR....RR............", // 0  quillas rojas arriba
    "...KRRRRRRRRRRK........",  // 1  corona roja con borde negro
    "..KKKKKKKKKKKK........",   // 2  cabeza negra
    "..KKKKWWKKWWKKKK........", // 3  ojos
    "..KKKKPPPPPPKKKK........", // 4  hocico
    "..KKKPPPPPPPPKKK........", // 5
    ".KKKRRWWWWWWRRKKK.......", // 6  pecho blanco con toque rojo
    ".KKKKKWWWWWWKKKKK.......", // 7  hombros
    "..KKKKKKWWKKKKKK........", // 8  torso alto
    "..KKKYYKK..KKYYKK.......", // 9  aros amarillos en muñecas
    "..KKKKWWKK..KKWWKK......", // 10 guantes
    "...KKKKKKKKKK...........", // 11 cintura
    ".....KKKK..KKKK.........", // 12 pelvis
    ".....KKKK..KKKK.........", // 13 pierna izq/der (alta)
    ".....KKKK..KKKK.........", // 14
    ".....RKKK..KKKR.........", // 15 detalle rojo en piernas
    ".....KKKK..KKKK.........", // 16
    ".....KKKK..KKKK.........", // 17
    ".....YYKK..KKYY...........",// 18 aros de tobillo
    "....WWWKK..KKWWW..........",// 19 parte alta de zapatillas
    "...RRRRKK..KKRRRR........", // 20 zapatillas rojas
    "...RRRRKK..KKRRRR........", // 21
    "...WWWWKK..KKWWWW........", // 22 suela blanca
    "........................"   // 23 base
    };

    const int gridH = (int)sprite.size();
    const int gridW = 16; // todas las filas miden 16

    // ===== Mapeo a NDC con celdas cuadradas y margen =====
    const float margin = 0.10f; // 10% margen
    const float cell = 2.0f * (1.0f - margin) / float(std::max(gridW, gridH));
    const float totalW = gridW * cell, totalH = gridH * cell;
    const float originX = -totalW * 0.5f, originY = -totalH * 0.5f;

    auto isFilled = [&](int x, int y)->bool {
        if (x < 0 || x >= gridW || y < 0 || y >= gridH) return false;
        return sprite[y][x] != '.';
        };

    // ===== Vértices de pixeles (GL_POINTS) =====
    std::vector<float> vPoints; vPoints.reserve(6 * gridW * gridH);
    auto addPoint = [&](float cx, float cy, RGB c) {
        vPoints.insert(vPoints.end(), { cx,cy,0.0f, c.r,c.g,c.b });
        };
    for (int gy = 0; gy < gridH; ++gy) {
        for (int gx = 0; gx < gridW; ++gx) {
            char ch = sprite[gy][gx];
            if (ch == '.') continue;
            float x = originX + (gx + 0.5f) * cell;
            float y = originY + ((gridH - 1 - gy) + 0.5f) * cell; // invertir Y para que fila 0 sea arriba
            addPoint(x, y, colorOf(ch));
        }
    }

#if DRAW_CONTOUR
    // ===== Contorno (GL_LINES) en bordes expuestos =====
    std::vector<float> vLines; vLines.reserve(6 * gridW * gridH * 4);
    auto addLine = [&](float x0, float y0, float x1, float y1, RGB c) {
        vLines.insert(vLines.end(), { x0,y0,0.0f, c.r,c.g,c.b, x1,y1,0.0f, c.r,c.g,c.b });
        };
    RGB edgeCol = { 0.90f, 0.05f, 0.05f }; // borde rojizo sutil
    for (int gy = 0; gy < gridH; ++gy) {
        for (int gx = 0; gx < gridW; ++gx) {
            if (!isFilled(gx, gy)) continue;
            float x0 = originX + gx * cell, x1 = x0 + cell;
            float y0 = originY + (gridH - 1 - gy) * cell, y1 = y0 + cell;
            if (!isFilled(gx - 1, gy)) addLine(x0, y0, x0, y1, edgeCol); // izq
            if (!isFilled(gx + 1, gy)) addLine(x1, y0, x1, y1, edgeCol); // der
            if (!isFilled(gx, gy + 1)) addLine(x0, y0, x1, y0, edgeCol); // abajo pantalla
            if (!isFilled(gx, gy - 1)) addLine(x0, y1, x1, y1, edgeCol); // arriba pantalla
        }
    }
#endif

    // ===== Subir a GPU =====
    GLuint VAO_pts, VBO_pts;
    glGenVertexArrays(1, &VAO_pts);
    glGenBuffers(1, &VBO_pts);
    glBindVertexArray(VAO_pts);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pts);
    glBufferData(GL_ARRAY_BUFFER, vPoints.size() * sizeof(float), vPoints.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#if DRAW_CONTOUR
    GLuint VAO_lin, VBO_lin;
    glGenVertexArrays(1, &VAO_lin);
    glGenBuffers(1, &VBO_lin);
    glBindVertexArray(VAO_lin);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_lin);
    glBufferData(GL_ARRAY_BUFFER, vLines.size() * sizeof(float), vLines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#endif

    // ===== Loop =====
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();

        // Tamaño del “pixel”: calcula según framebuffer
        int fbw, fbh; glfwGetFramebufferSize(window, &fbw, &fbh);
        float cellPx = (std::min(fbw, fbh) * (1.0f - margin)) / float(std::max(gridW, gridH));

        glBindVertexArray(VAO_pts);
        glPointSize(std::max(1.0f, cellPx * 0.85f)); // cuadraditos bien visibles
        glDrawArrays(GL_POINTS, 0, (GLsizei)(vPoints.size() / 6));
        glBindVertexArray(0);

#if DRAW_CONTOUR
        glBindVertexArray(VAO_lin);
        glLineWidth(std::max(1.0f, cellPx * 0.15f));
        glDrawArrays(GL_LINES, 0, (GLsizei)(vLines.size() / 6));
        glBindVertexArray(0);
#endif

        glfwSwapBuffers(window);
    }

#if DRAW_CONTOUR
    glDeleteBuffers(1, &VBO_lin);
    glDeleteVertexArrays(1, &VAO_lin);
#endif
    glDeleteBuffers(1, &VBO_pts);
    glDeleteVertexArrays(1, &VAO_pts);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
