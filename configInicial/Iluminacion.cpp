//Siliano Haller Rodrigo
//No.Cuenta: 319039627
//Practica 8
//Fecha de entrega: 17/10/2025




#include <string>
#include <iostream>
#include <algorithm>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>   

// Window props
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool    keys[1024] = { false };
GLfloat lastX = 400, lastY = 300;
bool    firstMouse = true;

// Time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// ===== Arco suave y control =====
const float SKY_RADIUS = 12.0f;   // radio de la “cúpula”
const float ANG_SPEED = 0.6f;    // rad/s

// Ángulos iniciales: sol en -90° (izq), luna en +90° (der)
float sunTheta = -glm::half_pi<float>();
float moonTheta = glm::half_pi<float>();

// Posiciones (se actualizan cada frame)
glm::vec3 sunPos = glm::vec3(0.0f);
glm::vec3 moonPos = glm::vec3(0.0f);

// Alturas máximas en el cenit
const float SUN_APEX_Y = 8.0f;
const float MOON_APEX_Y = 8.0f;

// Modo de transición actual
enum class CelestialMode { Idle, SunUp, MoonUp };
CelestialMode mode = CelestialMode::Idle;
// =================================

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 - Siliano Haller Rodrigo", nullptr, nullptr);
    if (!window) { std::cout << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cout << "Failed to initialize GLEW\n"; return EXIT_FAILURE; }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders (deben coincidir con uniforms: model/view/projection, light, light2, material, viewPos)
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Modelos
    Model piso((char*)"Models/pasto.obj");
    Model tronco((char*)"Models/tronco.obj");
    Model oso((char*)"Models/oso.obj");
    Model pino((char*)"Models/pino.obj");
    Model linterna((char*)"Models/linterna.obj");
    Model mochila((char*)"Models/mochila.obj");
    Model cuaderno((char*)"Models/cuaderno.obj");
    Model luna((char*)"Models/Luna.obj");
    Model sol((char*)"Models/sol.obj");

    // Proyección (FOV de la cámara en grados ? radianes)
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        // Time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        glfwPollEvents();
        DoMovement();

        // Clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Camera matrices
        glm::mat4 view = camera.GetViewMatrix();

        // --- Lighting setup ---
        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Posiciones de luces = posiciones de modelos
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"),
            sunPos.x, sunPos.y, sunPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"),
            moonPos.x, moonPos.y, moonPos.z);

        // Colores base (ajusta a gusto)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.35f, 0.32f, 0.28f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.00f, 0.95f, 0.85f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.00f, 1.00f, 1.00f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.08f, 0.10f, 0.18f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.30f, 0.35f, 0.60f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.60f, 0.70f, 1.00f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // Helper para setear model
        auto setModel = [&](const glm::mat4& m) {
            glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m));
            };

        // Escena
        setModel(glm::mat4(1.0f)); piso.Draw(lightingShader);
        setModel(glm::mat4(1.0f)); tronco.Draw(lightingShader);
        setModel(glm::mat4(1.0f)); pino.Draw(lightingShader);
        setModel(glm::mat4(1.0f)); oso.Draw(lightingShader);
        setModel(glm::mat4(1.0f)); linterna.Draw(lightingShader);
        setModel(glm::mat4(1.0f)); mochila.Draw(lightingShader);
        setModel(glm::mat4(1.0f)); cuaderno.Draw(lightingShader);

        // Sol
        {
            glm::mat4 m(1.0f);
            m = glm::translate(m, sunPos);
            m = glm::scale(m, glm::vec3(0.9f));
            setModel(m);
            sol.Draw(lightingShader);
        }
        // Luna
        {
            glm::mat4 m(1.0f);
            m = glm::translate(m, moonPos);
            m = glm::scale(m, glm::vec3(1.0f));
            setModel(m);
            luna.Draw(lightingShader);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    // Cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_RIGHT])                     camera.ProcessKeyboard(RIGHT, deltaTime);

    // Movimiento en arco según modo
    const float TMIN = -glm::half_pi<float>(); // -90°
    const float TMAX = glm::half_pi<float>(); // +90°

    switch (mode)
    {
    case CelestialMode::SunUp:
        // Sol sube de -90° a +90°, Luna baja de +90° a -90°
        sunTheta = std::min(sunTheta + ANG_SPEED * deltaTime, TMAX);
        moonTheta = std::max(moonTheta - ANG_SPEED * deltaTime, TMIN);
        if (sunTheta >= TMAX && moonTheta <= TMIN) mode = CelestialMode::Idle;
        break;

    case CelestialMode::MoonUp:
        // Luna sube de -90° a +90°, Sol baja de +90° a -90°
        moonTheta = std::min(moonTheta + ANG_SPEED * deltaTime, TMAX);
        sunTheta = std::max(sunTheta - ANG_SPEED * deltaTime, TMIN);
        if (moonTheta >= TMAX && sunTheta <= TMIN) mode = CelestialMode::Idle;
        break;

    case CelestialMode::Idle:
    default:
        break;
    }

    // Proyección del arco (x = R*sin(t), y = R*cos(t) - (R - apexY))
    sunPos.x = SKY_RADIUS * std::sin(sunTheta);
    sunPos.y = SKY_RADIUS * std::cos(sunTheta) - (SKY_RADIUS - SUN_APEX_Y);
    sunPos.z = 0.0f;

    moonPos.x = SKY_RADIUS * std::sin(moonTheta);
    moonPos.y = SKY_RADIUS * std::cos(moonTheta) - (SKY_RADIUS - MOON_APEX_Y);
    moonPos.z = 0.0f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_O) {
            // Sale el SOL (O)
            sunTheta = -glm::half_pi<float>(); // borde izq (horizonte)
            moonTheta = glm::half_pi<float>(); // borde der (baja)
            mode = CelestialMode::SunUp;
        }
        if (key == GLFW_KEY_L) {
            // Sale la LUNA (L)
            moonTheta = glm::half_pi<float>(); // borde der (horizonte)
            sunTheta = -glm::half_pi<float>(); // borde izq (baja)
            mode = CelestialMode::MoonUp;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = (GLfloat)xPos; lastY = (GLfloat)yPos; firstMouse = false; }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos; // invertido

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


