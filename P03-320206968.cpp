//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		0.0f, 0.0f, 0.0f,	//0
		1.0f, 0.0f, 0.0f,	//1
		0.5f, 0.0f, 0.866f,	//2
		0.5f, 0.816f, 0.289f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

// Pirámide triangular regular
void CrearPiramideTriangular2Colores()
{
	// --- Coordenadas de un tetraedro regular (lado = 1
	GLfloat v[] = {
		// --- Cara base (AZUL) ---
		0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   // v0
		1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   // v1
		0.5f, 0.0f, 0.866f,  0.0f, 0.0f, 1.0f,   // v2

		// --- Cara 1 (ROJO) ---
		0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   // v0
		1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   // v1
		0.5f, 0.816f, 0.289f,1.0f, 0.0f, 0.0f,   // v3 (punta)

		// --- Cara 2 (VERDE) ---
		1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   // v1
		0.5f, 0.0f, 0.866f,  0.0f, 1.0f, 0.0f,   // v2
		0.5f, 0.816f, 0.289f,0.0f, 1.0f, 0.0f,   // v3

		// --- Cara 3 (AMARILLO) ---
		0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,   // v0
		0.5f, 0.0f, 0.866f,  1.0f, 1.0f, 0.0f,   // v2
		0.5f, 0.816f, 0.289f,1.0f, 1.0f, 0.0f    // v3
	};

	// Índices(4 triángulos, 12 vértices únicos)
	unsigned int idx[] = {
		 0,  1,  2,   // base
		 3,  4,  5,   // cara 1
		 6,  7,  8,   // cara 2
		 9, 10, 11    // cara 3
	};

	Mesh* m = new Mesh();
	m->CreateMeshPosColor(v, idx, 12 * 6, 12);
	meshList.push_back(m);
}

void CrearPiramideTriangularInvertidaOrigenPunta()
{
	// --- Punta en el origen (0,0,0), base ajustada
	GLfloat v[] = {
		// --- Cara base (AZUL) ---
	   -0.5f, 0.816f, -0.289f,   0.0f, 0.0f, 1.0f, // v0
		0.5f, 0.816f, -0.289f,   0.0f, 0.0f, 1.0f, // v1
		0.0f, 0.816f,  0.577f,   0.0f, 0.0f, 1.0f, // v2

		// --- Cara 1 (ROJO) ---
	   -0.5f, 0.816f, -0.289f,   1.0f, 0.0f, 0.0f, // v0
		0.5f, 0.816f, -0.289f,   1.0f, 0.0f, 0.0f, // v1
		0.0f, 0.0f,  0.0f,       1.0f, 0.0f, 0.0f, // punta (origen)

		// --- Cara 2 (VERDE) ---
		0.5f, 0.816f, -0.289f,   0.0f, 1.0f, 0.0f, // v1
		0.0f, 0.816f,  0.577f,   0.0f, 1.0f, 0.0f, // v2
		0.0f, 0.0f,  0.0f,       0.0f, 1.0f, 0.0f, // punta

		// --- Cara 3 (AMARILLO) ---
	   -0.5f, 0.816f, -0.289f,   1.0f, 1.0f, 0.0f, // v0
		0.0f, 0.816f,  0.577f,   1.0f, 1.0f, 0.0f, // v2
		0.0f, 0.0f,  0.0f,       1.0f, 1.0f, 0.0f  // punta
	};

	unsigned int idx[] = {
		 0, 1, 2,  // base
		 3, 4, 5,  // cara 1
		 6, 7, 8,  // cara 2
		 9,10,11   // cara 3
	};

	Mesh* m = new Mesh();
	m->CreateMeshPosColor(v, idx, 12 * 6, 12);
	meshList.push_back(m);
}

void CrearAristasPiramideTriangular()
{
	// Mismos vértices que la pirámide
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f,   // v0
		1.0f, 0.0f, 0.0f,   // v1
		0.5f, 0.0f, 0.866f, // v2
		0.5f, 0.816f, 0.289f // v3
	};

	// Aristas como pares de vértices
	unsigned int indices[] = {
		0, 1,   // base v0-v1
		1, 2,   // base v1-v2
		2, 0,   // base v2-v0
		0, 3,   // lados
		1, 3,
		2, 3
	};

	Mesh* edges = new Mesh();
	edges->CreateMesh(vertices, indices, 12, 12); 
	meshList.push_back(edges);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();                  // 0
	CrearPiramideTriangular();    // 1
	CrearPiramideTriangular2Colores();   // 2
	CrearPiramideTriangularInvertidaOrigenPunta(); //3
	CrearAristasPiramideTriangular(); //4
	CreateShaders();

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
												//velocidada de movimiento y giro (2 ultimos parametros)
	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//PIRAMIDE TRIANGULAR GRANDE
		model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirámide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera

		//Dibujar aristas negras
		glLineWidth(50.0f); // grosor 
		glUniform3f(uniformColor, 0.0f, 0.0f, 0.0f);
		meshList[2]->RenderMeshLines();
		glLineWidth(1.0f); // restaurar grosor


		shaderList[1].useShader(); // <- usa el shader con color por vértice
		GLuint uModel = shaderList[1].getModelLocation();
		GLuint uProj = shaderList[1].getProjectLocation();
		GLuint uView = shaderList[1].getViewLocation();


		//====================================================================================
		//Piramide chiquita esquina abajo 1 - INSTANCIA 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.001f, -0.0001f, -4.0001f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita esquina 2- INSTANCIA 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.34f, -0.0001f, -4.0f+0.59f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita entre 1 y 2 - INSTANCIA 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.17f, -0.0001f, -4.0f + 0.295f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita //PUNTA - INSTANCIA 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.34f, 0.56f, -4.0f + 0.195f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita esquina 3 - INSTANCIA 5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.685f, -0.0001f, -4.0001f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita entre 2 y 3 - INSTANCIA 6
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.51f, -0.0001f, -4.0f + 0.295f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita entre 1 y 3 - INSTANCIA 7
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.34f, -0.0001f, -4.0001f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita esquina 5 - INSTANCIA 8
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.17f, 0.282f, -4.0001f + 0.099));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita esquina 6 - INSTANCIA 9
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.34f, 0.282f, -4.0f + 0.39f));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita esquina 7 - INSTANCIA 10
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.51f, 0.282f, -4.0001f + 0.099));
		model = glm::scale(model, glm::vec3(0.32, 0.32, 0.32));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();


		//Piramide chiquita esquina abajo 1 - INSTANCIA 11
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.48f, -0.001f, -4.0001f + 0.275));
		model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0)); // rotacion
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita esquina abajo 2 - INSTANCIA 12
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.822f, -0.001f, -4.0001f + 0.275));
		model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0)); // rotacion
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita esquina abajo 2 - INSTANCIA 13
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.652f, -0.001f, -4.0001f + 0.572));
		model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0)); // rotacion
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Piramide chiquita  centro amarillo 1 - INSTANCIA 14
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.16f, 0.025f, -4.0001f+0.285));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		//model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f)); // reflejar en XZ
		model = glm::rotate(model, glm::radians(320.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(8.0f), glm::vec3(1, 0, 0));
		//model = glm::translate(model, glm::vec3(0.8f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		//Piramide chiquita  centro amarillo 2 - INSTANCIA 15
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.33f, 0.025f, -4.0001f + 0.585));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(320.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(8.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		//Piramide chiquita  centro amarillo 3 - INSTANCIA 16
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.34f, 0.29f, -4.0001f + 0.39));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(320.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(8.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		//Piramide chiquita  centro rojo 1 - INSTANCIA 17
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.33f, 0.02f, -4.0001f+0.005));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		//Piramide chiquita  centro rojo 2 - INSTANCIA 18
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.3f, -4.0001f + 0.1));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		//Piramide chiquita  centro rojo 3 - INSTANCIA 19
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.67f, 0.02f, -4.0001f + 0.005));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();


		//Piramide chiquita  centro verde 3 - INSTANCIA 20
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.85f, 0.02f, -4.0001f + 0.282));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		//Piramide chiquita  centro verde 3 - INSTANCIA 21
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.675f, 0.02f, -4.0001f + 0.58));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		//Piramide chiquita  centro verde 3 - INSTANCIA 22
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.675f, 0.3f, -4.0001f + 0.38));
		model = glm::scale(model, glm::vec3(0.29, 0.29, 0.29));
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[3]->RenderMesh();

		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		