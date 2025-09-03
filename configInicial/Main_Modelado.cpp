//Siliano Haller Rodrigo
//Numero de cuenta: 319039627
//Practica 4
//Fecha de entrega: 05/09/2025




#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 - Siliano Haller Rodrigo", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection


	float vertices[] = {
		// Frente
		-0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		-0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		-0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,

		// Atrás
		-0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		-0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		-0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,

		// Derecha
		 0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,

		 // Izquierda
		 -0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,

		 // Abajo
		 -0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		  0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		  0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		  0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.8f,

		 // Arriba
		 -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
		  0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		  0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.8f,
		 -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.8f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	// Vértices de los ojos triangulares
	float eyeVertices[] = {
		// Triángulo izquierdo (ojo izquierdo)
		-0.05f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,  // Vértice 1 (rojo)
		0.05f,  0.1f, 0.0f,    1.0f, 0.0f, 0.0f,  // Vértice 2 (rojo)
		0.0f,   0.2f, 0.0f,    1.0f, 0.0f, 0.0f,  // Vértice 3 (rojo)

		// Triángulo derecho (ojo derecho)
		-0.05f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,  // Vértice 1 (rojo)
		0.05f,  0.1f, 0.0f,    1.0f, 0.0f, 0.0f,  // Vértice 2 (rojo)
		0.0f,   0.2f, 0.0f,    1.0f, 0.0f, 0.0f,  // Vértice 3 (rojo)
	};

	GLuint eyeVBO, eyeVAO;
	glGenVertexArrays(1, &eyeVAO);
	glGenBuffers(1, &eyeVBO);

	glBindVertexArray(eyeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, eyeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(eyeVertices), eyeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);  // Vértices
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));  // Colores
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		

		ourShader.Use();

		// Matrices base
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		// ?? Como lo tenías: primero traslación, después rotación
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		// (Recomendado: obtener estos locations una sola vez fuera del loop)
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uUseSolidColor = glGetUniformLocation(ourShader.Program, "useSolidColor");
		GLint uSolidColor = glGetUniformLocation(ourShader.Program, "solidColor");

		// Sube projection y view
		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);

		// --- 1) CUERPO: morado usando VBO ---
		glUniform1i(uUseSolidColor, GL_FALSE);   // usar colores del arreglo de vértices
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		 //--- 2) OJOS: color sólido rojo ---
		glUniform1i(uUseSolidColor, GL_TRUE);
		glUniform3f(uSolidColor, 1.0f, 0.1f, 0.1f);

		// Ojo izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, 0.15f, 0.41f));
		model = glm::scale(model, glm::vec3(0.18f, 0.12f, 0.06f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Ojo derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, 0.15f, 0.41f));
		model = glm::scale(model, glm::vec3(0.18f, 0.12f, 0.06f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//BOCA
//
		glUniform1i(uUseSolidColor, GL_TRUE);      // Usamos color sólido
		glUniform3f(uSolidColor, 1.0f, 1.0f, 1.0f); // Blanco

		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, -0.25f, 0.41f)); // Ajuste en Y y Z para la primera división
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.06f));      // tamaño de la primera división
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Divisiones siguientes (similares en tamaño y ajustando la posición en Y)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, -0.25f, 0.41f)); // Segunda división
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.06f));      // tamaño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.41f));  // Tercera división
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.06f));      // tamaño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, -0.25f, 0.41f));  // Cuarta división
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.06f));      // tamaño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, -0.25f, 0.41f));  // Quinta división
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.06f));      // tamaño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, -0.25f, 0.41f));  // Sexta división
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.06f));      // tamaño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- 3) PICOS SUPERIORES ---

		glUniform1i(uUseSolidColor, GL_TRUE);
		glUniform3f(uSolidColor, 0.4f, 0.0f, 0.65f); // morado oscuro para los picos

		// Pico central (más grande y más angulado hacia atrás)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, 0.0f)); // posición más alta
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // mayor inclinación hacia atrás
		model = glm::scale(model, glm::vec3(0.25f, 0.65f, 0.1f)); // mayor tamaño en altura y longitud
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pico izquierdo (más grande, angulado y hacia afuera)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.45f, 0.55f, 0.0f)); // desplazado a la izquierda
		model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // mayor inclinación hacia atrás
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // apertura hacia afuera
		model = glm::scale(model, glm::vec3(0.25f, 0.7f, 0.1f)); // tamaño más grande y alargado
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pico derecho (simétrico al izquierdo, más grande y angulado)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, 0.55f, 0.0f)); // desplazado a la derecha
		model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // mayor inclinación hacia atrás
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // apertura hacia afuera
		model = glm::scale(model, glm::vec3(0.25f, 0.7f, 0.1f)); // tamaño más grande y alargado
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
//
		//Picos traseros

		glUniform1i(uUseSolidColor, GL_TRUE);
		glUniform3f(uSolidColor, 0.4f, 0.0f, 0.65f); // morado oscuro para los picos

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, -0.5f)); // Pico central desplazado hacia atrás en Z
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pico superior (arriba del centro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, -0.5f)); // Desplazado hacia arriba
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		// Pico izquierdo (a la izquierda del centro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.3f, 0.0f, -0.5f)); // Desplazado hacia la izquierda
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pico derecho (a la derecha del centro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, -0.5f)); // Desplazado hacia la derecha
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pico superior izquierdo (diagonal hacia la parte superior izquierda)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.3f, 0.3f, -0.5f)); // Desplazado hacia arriba y a la izquierda
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pico superior derecho (diagonal hacia la parte superior derecha)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, 0.3f, -0.5f)); // Desplazado hacia arriba y a la derecha
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		// --- 5) MANOS ---
// Definir color para las manos (morado oscuro, el mismo que los picos)
		glUniform1i(uUseSolidColor, GL_TRUE);
		glUniform3f(uSolidColor, 0.4f, 0.0f, 0.65f); // morado oscuro para las manos

		// Mano izquierda (lado izquierdo del cubo)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.6f, -0.3f, 0.0f)); // Posicionada a la izquierda y hacia abajo
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.05f)); // Tamaño de la palma de la mano
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja la palma de la mano

		// Dedos de la mano izquierda (3 dedos)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.6f, -0.25f, 0.07f)); // Posición para los dedos (saliendo de la palma)
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.05f)); // Tamaño de cada dedo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el primer dedo

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.55f, -0.25f, 0.07f)); // Desplazado ligeramente
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.05f)); // Tamaño del dedo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el segundo dedo

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.25f, 0.07f)); // Desplazado ligeramente
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.05f)); // Tamaño del dedo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el tercer dedo

		// Mano derecha (lado derecho del cubo)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, -0.3f, 0.0f)); // Posicionada a la derecha y hacia abajo
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.05f)); // Tamaño de la palma de la mano
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja la palma de la mano

		// Dedos de la mano derecha (3 dedos)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, -0.25f, 0.07f)); // Posición para los dedos (saliendo de la palma)
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.05f)); // Tamaño de cada dedo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el primer dedo

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, -0.25f, 0.07f)); // Desplazado ligeramente
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.05f)); // Tamaño del dedo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el segundo dedo

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.25f, 0.07f)); // Desplazado ligeramente
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.05f)); // Tamaño del dedo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el tercer dedo

		//Patas

		glUniform1i(uUseSolidColor, GL_TRUE);
		glUniform3f(uSolidColor, 0.4f, 0.0f, 0.65f); // morado oscuro para las patas

		// Pata izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, -0.6f, 0.0f)); // Desplazado hacia la izquierda en X y hacia abajo en Y
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f)); // Tamaño de la base de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja la base de la pata

		// Cuatro pequeños bloques (pies) debajo de la pata izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.35f, -0.75f, 0.0f)); // Desplazado hacia la izquierda y hacia abajo
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Tamaño pequeño para los pies
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el pie 1

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, -0.75f, 0.0f)); // Pie 2
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el pie 2

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.15f, -0.75f, 0.0f)); // Pie 3
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el pie 3

		

		// Pata derecha (simétrica a la izquierda)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, -0.6f, 0.0f)); // Desplazado hacia la derecha en X y hacia abajo en Y
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f)); // Tamaño de la base de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja la base de la pata

		// Cuatro pequeños bloques (pies) debajo de la pata derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, -0.75f, 0.0f)); // Desplazado hacia la derecha y hacia abajo
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Tamaño pequeño para los pies
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el pie 1

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, -0.75f, 0.0f)); // Pie 2
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el pie 2

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f, -0.75f, 0.0f)); // Pie 3
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Tamaño pequeño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja el pie 3

		


		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.4f;
 }


