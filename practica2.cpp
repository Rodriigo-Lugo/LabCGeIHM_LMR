//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* vShaderRed = "shaders/shader_red.vert";
static const char* fShaderRed = "shaders/shader_red.frag";
static const char* fShaderBlue = "shaders/shader_blue.frag";
static const char* fShaderBrown = "shaders/shader_brown.frag";
static const char* fShaderDarkgreen = "shaders/shader_darkgreen.frag";
static const char* fShaderGreen = "shaders/shader_green.frag";
//shaders nuevos se crearían acá

float angulo = 45.0f;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
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
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
	meshList.push_back(cubo);
}

void CrearPiramideCuadrada()
{
	GLfloat vertices[] = {
		// Base
		-0.5f, -0.5f,  0.5f,   // 0
		 0.5f, -0.5f,  0.5f,   // 1
		 0.5f, -0.5f, -0.5f,   // 2
		-0.5f, -0.5f, -0.5f,   // 3

		// Punta
		 0.0f,  0.5f,  0.0f    // 4
	};

	unsigned int indices[] = {
		// Base
		0, 1, 2,
		2, 3, 0,

		// Lados
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	Mesh* piramide = new Mesh();
	piramide->CreateMesh(vertices, indices, 15, 18); // 5*3 floats = 15, 6 caras*3 = 18 índices
	meshList.push_back(piramide);
}


void CrearLetrasyFiguras()
{

	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};
	
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	GLfloat vertices_letraR[] = {
		// R (azul)
		// izquierda
		-0.6f, 0.3f, 0.5f,   0.0f, 0.0f, 1.0f,
		-0.5f, 0.3f, 0.5f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.2f, 0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, 0.3f, 0.5f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.2f, 0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.2f, 0.5f,  0.0f, 0.0f, 1.0f,
		// centro arriba
		-0.5f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		// centro derecha
		-0.4f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.4f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.4f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		// centro abajo
		-0.3f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		// parte de abajo
		-0.4f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.4f, -0.2f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.4f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.4f, -0.2f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.3f, -0.2f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	MeshColor* letraR = new MeshColor();
	letraR->CreateMeshColor(vertices_letraR, 30*6);
	meshColorList.push_back(letraR);

	GLfloat vertices_letraL[] = {
		// palito vertical
		-0.2f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.1f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.2f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.1f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.2f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.1f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
		// palito horizontal
		-0.1f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.1f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.1f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.1f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.1f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.1f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f
	};
	MeshColor* letraL = new MeshColor();
	letraL->CreateMeshColor(vertices_letraL, 12 * 6);
	meshColorList.push_back(letraL);

	GLfloat vertices_letraM[] = {
		// Palito izquierdo
		0.2f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.3f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.2f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.3f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.2f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.3f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		// Centro izquierda
		0.3f, 0.15f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.3f, 0.3f, 0.0f,    1.0f, 0.0f, 0.0f,
		0.4f, 0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.4f, 0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.4f, 0.2f, 0.0f,    1.0f, 0.0f, 0.0f,
		0.3f, 0.3f, 0.0f,    1.0f, 0.0f, 0.0f,
		// Centro derecha
		0.5f, 0.15f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.5f, 0.3f, 0.0f,    1.0f, 0.0f, 0.0f,
		0.4f, 0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.4f, 0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.4f, 0.2f, 0.0f,    1.0f, 0.0f, 0.0f,
		0.5f, 0.3f, 0.0f,    1.0f, 0.0f, 0.0f,
		// Palito derecho
		0.5f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.6f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.5f, 0.3f, 0.0f,    1.0f, 0.0f, 0.0f,
		0.6f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.5f, 0.3f, 0.0f,    1.0f, 0.0f, 0.0f,
		0.6f, 0.3f, 0.0f,    1.0f, 0.0f, 0.0f
	};
	MeshColor* letraM = new MeshColor();
	letraM->CreateMeshColor(vertices_letraM, 24 * 6);
	meshColorList.push_back(letraM);


}


void CreateShaders()
{
	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
	//azul [2]
	Shader* shaderBlue = new Shader();
	shaderBlue->CreateFromFiles(vShaderRed, fShaderBlue);
	shaderList.push_back(*shaderBlue);
	//verde oscuro [3]
	Shader* shaderDarkgreen = new Shader();
	shaderDarkgreen->CreateFromFiles(vShaderRed, fShaderDarkgreen);
	shaderList.push_back(*shaderDarkgreen);
	//rojo [4]
	Shader* shaderRed = new Shader();
	shaderRed->CreateFromFiles(vShaderRed, fShaderRed);
	shaderList.push_back(*shaderRed);
	//verde [5]
	Shader* shaderGreen = new Shader();
	shaderGreen->CreateFromFiles(vShaderRed, fShaderGreen);
	shaderList.push_back(*shaderGreen);
	//cafe [6]
	Shader* shaderBrown = new Shader();
	shaderBrown->CreateFromFiles(vShaderRed, fShaderBrown);
	shaderList.push_back(*shaderBrown);

}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	CreaPiramide();
	CrearCubo();
	CrearPiramideCuadrada();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		//PARTE DEL DIBUJADO DE LETRAS COMENTADO
		/*
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Letra R
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();
		//Letra L
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();
		//Letra M
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();*/

		/*
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();*/

		//angulo += 0.01;

		//cubo rojo
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		//cubo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.45f, -3.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//cubo verde ventana
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		//cubo ventana izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.41f, -0.29f, -3.21f));
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//cubo ventana derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.12f, -0.30f, -2.9f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo - 26.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//cubo puerta
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, -0.761f, -3.11f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo - 29.6f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para la pirámide del techo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		//Piramide
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		//cubo cafe
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		//cubo tronco1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.775f, -0.73f, -2.5f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.25f));
		model = glm::rotate(model, glm::radians(angulo + 25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//cubo tronco 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.775f, -0.73f, -2.5f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.25f));
		model = glm::rotate(model, glm::radians(angulo - 25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para la pirámide del techo
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		//Piramide arbol 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.775f, -0.43f, -2.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.45f, 0.25f));
		model = glm::rotate(model, glm::radians(angulo+25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		//Piramide arbol2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.775f, -0.43f, -2.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.45f, 0.25f));
		model = glm::rotate(model, glm::radians(angulo - 25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
