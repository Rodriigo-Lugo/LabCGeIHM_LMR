/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza=true;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture octaedroTexture;

Texture LetrasPTexture;



Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Model CuerpoDragon_M;
Model AlaIzq_M;
Model AlaDer_M;
Model Arco_M;
Model Letrero_M;
Model Puerta1_M;
Model Puerta2_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;



// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = { //0
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {//1
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {//2
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {//3
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {// para mostrar todos los números
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {// para mostrar un solo número
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	// Letrero: reutilizable del tamaño de UNA celda del atlas
	// - 13 columnas -> ancho por celda = 1/13
	// - banda útil en V (alto de cada fila) = 0.31640625
	// - margen (MARGEN_UV) para evitar ver bordes entre celdas
	const float ANCHO_CELDA_U = 1.0f / 13.0f;   //  0.07692308
	const float ALTO_CELDA_V = 0.31640625f;    // 162/512
	const float MARGEN_UV = 1.0f / 512.0f;  //  0.001953125

	unsigned int letreroIndices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLfloat letreroVertices[] = {
		//     x      y     z         u                 v               nx    ny    nz
		-0.5f,  0.0f,  0.5f,   0.0f + MARGEN_UV,   0.0f + MARGEN_UV,    0.0f, -1.0f, 0.0f, // Inferior Izq
		 0.5f,  0.0f,  0.5f,   ANCHO_CELDA_U - MARGEN_UV, 0.0f + MARGEN_UV,  0.0f, -1.0f, 0.0f, // Inferior Der
		 0.5f,  0.0f, -0.5f,   ANCHO_CELDA_U - MARGEN_UV, ALTO_CELDA_V - MARGEN_UV, 0.0f, -1.0f, 0.0f, // Superior Der
		-0.5f,  0.0f, -0.5f,   0.0f + MARGEN_UV,   ALTO_CELDA_V - MARGEN_UV, 0.0f, -1.0f, 0.0f  // Superior Izq
	};


	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

	Mesh* LetreroP = new Mesh(); //Posicion meshList[7]
	LetreroP->CreateMesh(letreroVertices, letreroIndices, 32, 6);
	meshList.push_back(LetreroP);

}


void CrearOctaedro()
{
	// 8 caras (triangulos), 3 vertices por cara = 24 vertices
	// Formato: x, y, z,  S, T,  NX, NY, NZ
	const float n = 1.0f; // 
	const float s = 0.5f; // radio

	// Puntos UV
	const float U1 = 0.00f, V1 = 0.00f; // 1
	const float U2 = 0.00f, V2 = 0.40f; // 2
	const float U3 = 0.33f, V3 = 0.20f; // 3
	const float U4 = 0.33f, V4 = 0.60f; // 4
	const float U5 = 0.33f, V5 = 0.99f; // 5
	const float U6 = 0.66f, V6 = 0.00f; // 6
	const float U7 = 0.66f, V7 = 0.40f; // 7
	const float U8 = 0.66f, V8 = 0.79f; // 8 
	const float U9 = 0.99f, V9 = 0.60f; // 9
	const float U10 = 0.99f, V10 = 0.99f; // 10

	// Indices
	unsigned int oct_indices[] = {
		0,1,2,  3,4,5,  6,7,8,  9,10,11,
		12,13,14,  15,16,17,  18,19,20,  21,22,23
	};

	GLfloat oct_vertices[] = {
		//X	   Y      Z        U       V                    NX      NY      NZ
		// ---- Cara 1  (triangulo "1" -> puntos 1,2,3)
		0.0f,  s,   0.0f,   U3 - 0.01f, V3,                 -n,    -n,    -n,
		0.0f,  0.0f, s,     U2 + 0.01f, V2 - 0.01f,         -n,    -n,    -n,
		s,     0.0f, 0.0f,  U1 + 0.01f, V1 + 0.01f,         -n,    -n,    -n,
		// ---- Cara 2  (triangulo "2" -> puntos 2,4,3)
		0.0f,  s,   0.0f,   U3, V3 + 0.01f,                  +n,    -n,    -n,
		-s,    0.0f, 0.0f,  U4, V4 - 0.01f,                  +n,    -n,    -n,
		0.0f,  0.0f, s,     U2 + 0.01f, V2,                  +n,    -n,    -n,
		// ---- Cara 3  (triangulo "3" -> puntos 4,7,3)
		0.0f,  s,    0.0f,  U3 + 0.01f, V3 + 0.01f,          +n,    -n,    +n,
		0.0f,  0.0f, -s,    U7 - 0.01f, V7,                  +n,    -n,    +n,
		-s,    0.0f,  0.0f, U4 + 0.01f, V4 - 0.01f,          +n,    -n,    +n,
		// ---- Cara 4  (triangulo "4" -> puntos 6,7,3)
		0.0f,  s,   0.0f,   U3 + 0.01f, V3,                  -n,    -n,    +n,
		s,     0.0f, 0.0f,  U6 - 0.01f, V6 + 0.02f,          -n,    -n,    +n,
		0.0f,  0.0f, -s,    U7, V7 - 0.01f,                  -n,    -n,    +n,
		// ---- Cara 5  (triangulo "5" -> puntos 5,4,8)
		0.0f, -s,   0.0f,   U8 - 0.01f, V8 + 0.01f,          -n,    +n,    -n,
		s,     0.0f, 0.0f,  U5 + 0.01f, V5,                  -n,    +n,    -n,
		0.0f,  0.0f, s,     U4 + 0.01f, V4 + 0.01f,          -n,    +n,    -n,
		// ---- Cara 6  (triangulo "6" -> puntos 4,7,8)
		0.0f, -s,   0.0f,   U8, V8,                            +n,    +n,    -n,
		0.0f,  0.0f, s,     U4 + 0.01f, V4,                   +n,    +n,    -n,
		-s,    0.0f, 0.0f,  U7 - 0.01f, V7 + 0.015f,          +n,    +n,    -n,
		// ---- Cara 7  (triangulo "7" -> puntos 7,9,8)
		0.0f, -s,    0.0f,  U8 + 0.01f, V8,                   +n,    +n,    +n,
		-s,    0.0f,  0.0f, U7 + 0.01f, V7 + 0.01f,           +n,    +n,    +n,
		0.0f,  0.0f, -s,    U9, V9,                            +n,    +n,    +n,
		// ---- Cara 8  (triangulo "8" -> puntos 8,9,10)
		0.0f, -s,   0.0f,   U8 + 0.01f,  V8 + 0.01f,          -n,    +n,    +n,
		0.0f,  0.0f, -s,    U9,  V9 + 0.01f,                  -n,    +n,    +n,
		s,     0.0f,  0.0f, U10, V10,                          -n,    +n,    +n,
	};

	Mesh* oct = new Mesh();
	oct->CreateMesh(oct_vertices, oct_indices, 192, 24);
	meshList.push_back(oct);
}




void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearOctaedro();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	LetrasPTexture = Texture("Textures/Tipografia_Pokemon_GIMP.png");
	LetrasPTexture.LoadTextureA();
	octaedroTexture = Texture("Textures/octaedro_numeros_gimp.png");
	octaedroTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/17174_Tiamat_new.obj");
	CuerpoDragon_M = Model();
	CuerpoDragon_M.LoadModel("Models/CuerpoDragon.fbx");
	AlaIzq_M = Model();
	AlaIzq_M.LoadModel("Models/AlaIzquierdaDragon.fbx");
	AlaDer_M = Model();
	AlaDer_M.LoadModel("Models/AlaDerechaDragon.fbx");
	Arco_M = Model();
	Arco_M.LoadModel("Models/Arco.fbx");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/Letrero.fbx");
	Puerta1_M = Model();
	Puerta1_M.LoadModel("Models/Puerta1.fbx");
	Puerta2_M = Model();
	Puerta2_M.LoadModel("Models/Puerta2.fbx");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.1f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	//Matrices extras
	glm::mat4 modelL(1.0);
	glm::mat4 modelC(1.0);
	glm::mat4 modelR(1.0);
	glm::mat4 modelLetras(1.0);
	glm::mat4 modelOct(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	//Variables ejercicio 1:
	double tActual = 0.0;
	double tiempoAcum_Num = glfwGetTime();
	double intervaloCambio = 1.0; // 1 pasos por segundo para ser visible
	float pasoU = 0.25f;          // 4 numeros -> 0.25

	//Variable ejercicio 2:
	double tNow = 0.0;
	double tiempoAcum = glfwGetTime();
	double intervalo = 0.4;			// alterna cada X segundos
	bool cambiaNum = true;        // true -> Numero1, false -> Numero2

	//Mover dragon
	dragonavance = 0.0f;
	bool dragonAvanza = true; // true para ir hacia +20, false para regresar a 0
	float VelocidadDragon = 0.1f; // velocidad de movimiento
	

	//Mover alas dragon
	float flapAngulo = 0.0f;     // ángulo actual en grados
	bool flapSube = true;         // true=subiendo, false=bajando
	float flapVelocidadSubir = 1.0f; // grados al subir 
	float flapVelocidadBajar = 1.0f; // grados al bajar
	float flapMin = -5.0f; // limite inferior
	float flapMax = 40.0f; // limite superior

	// ===== VARIABELS PARA LETRERO =====
	const int CANT_CARACTERES = 16;   // "PROYECTO CGEIHC "

	// imagen tipografia Pokemon: 13 columnas, 2 filas
	const float CELDA_U = 1.0f / 13.0f; // ancho por columna
	const float BANDA_V_INFERIOR = 0.0f;  // fila inferior (N..Z)
	const float BANDA_V_SUPERIOR = 0.5f;  // fila superior (A..M)

	// Espaciado y velocidad
	const float separacion_letras = 1.5f; // separacion entre letras en mundo
	float velocidad_marquesina = 0.03f;   // letras por segundo

	// Estado
	int   indice_central = 0;     // indice de la letra central
	float avance_celda = 0.0f;  // rango 0..1, progreso entre celdas

	// Offsets UV por caracter de la frase "PROYECTO CGEIHC "
	// u = columna * CELDA_U, v = BANDA_V_INFERIOR o BANDA_V_SUPERIOR
	// Orden: P R O Y E C T O (espacio) C G E I H C (espacio final)
	float u_letras[CANT_CARACTERES] = {
		2 * CELDA_U,  // P  (inf, col 2)
		4 * CELDA_U,  // R  (inf, col 4)
		1 * CELDA_U,  // O  (inf, col 1)
		11 * CELDA_U, // Y  (inf, col 11)
		4 * CELDA_U,  // E  (sup, col 4)
		2 * CELDA_U,  // C  (sup, col 2)
		6 * CELDA_U,  // T  (inf, col 6)
		1 * CELDA_U,  // O  (inf, col 1)
		0 * CELDA_U,  // espacio
		2 * CELDA_U,  // C  (sup, col 2)
		6 * CELDA_U,  // G  (sup, col 6)
		4 * CELDA_U,  // E  (sup, col 4)
		8 * CELDA_U,  // I  (sup, col 8)
		7 * CELDA_U,  // H  (sup, col 7)
		2 * CELDA_U,  // C  (sup, col 2)
		0 * CELDA_U   // espacio final para separar la frase
	};

	float v_letras[CANT_CARACTERES] = {
		BANDA_V_INFERIOR, // P
		BANDA_V_INFERIOR, // R
		BANDA_V_INFERIOR, // O
		BANDA_V_INFERIOR, // Y
		BANDA_V_SUPERIOR, // E
		BANDA_V_SUPERIOR, // C
		BANDA_V_INFERIOR, // T
		BANDA_V_INFERIOR, // O
		BANDA_V_INFERIOR, // espacio
		BANDA_V_SUPERIOR, // C
		BANDA_V_SUPERIOR, // G
		BANDA_V_SUPERIOR, // E
		BANDA_V_SUPERIOR, // I
		BANDA_V_SUPERIOR, // H
		BANDA_V_SUPERIOR, // C
		BANDA_V_INFERIOR  // espacio final
	};

	// Punto base relativo a modelLetras 
	// usa baseZ en X del translate, baseX en Y y baseY en Z
	const float baseX = 0.0f;
	const float baseY = 0.0f;
	const float baseZ = 0.7f;
	const float desplazamientoX_inicial = 0.8f;

	// Rotaciones y escala para alinear al arco
	const glm::vec3 escala_letra(1.5f, 1.5f, 1.5f);
	const float rotZ_letra = -90.0f * toRadians;
	const float rotY_letra = 180.0f * toRadians;

	const int IDX_ESPACIO1 = 8;   // espacio en medio
	const int IDX_ESPACIO2 = 15;  // espacio final

	// 1) Índices basados en la letra central
	int idxIzq = 0.0;
	int idxCentro = 0.0;
	int idxDer = 0.0;

	// 2) Posiciones en X usando -avance (desplaza a la izquierda)
	float posCentro = 0.0f;
	float posIzq = 0.0f;
	float posDer = 0.0f;



	//=============VARIABLES PARA LAS PUERTAS===========
	// Estado general
	bool objetivo_puertas_abiertas = false;   //true = abrir, false = cerrar
	bool puertas_en_animacion = false;   // hay una animación de apertura/cierre en curso
	bool tecla_f_estado_anterior = false;   // para detectar el cambio de la tecla F

	// Puerta 1 (rotación sobre bisagra)
	float angulo_puerta1 = 0.0f;             // grados actuales, rango [0 a 90]
	const float velocidad_puerta1_grados = 4.0f;  // grados por segundo

	// Puerta 2 (deslizamiento en dos etapas: primero +Z, luego +X)
	float desplazamiento_puerta2_z = 0.0f;   // avance en Z del mundo, rango [0 a 2]
	float desplazamiento_puerta2_x = 0.0f;   // avance en X del mundo, rango [0 a 4]
	const float velocidad_puerta2_z = 0.1f;  // velocidad de mover en Z del mundo
	const float velocidad_puerta2_x = 0.1f;  // velocidad de mover en X del mundo

	// ===== VARIABLES DE TIRADA DEL OCTAEDRO =====
	// ===== caida + giro + cara aleatoria arriba =====
	// Normales de las 8 caras 
	const glm::vec3 normales_octa[8] = {
		glm::normalize(glm::vec3(+1, +1, +1)),
		glm::normalize(glm::vec3(+1, +1, -1)),
		glm::normalize(glm::vec3(+1, -1, +1)),
		glm::normalize(glm::vec3(+1, -1, -1)),
		glm::normalize(glm::vec3(-1, +1, +1)),
		glm::normalize(glm::vec3(-1, +1, -1)),
		glm::normalize(glm::vec3(-1, -1, +1)),
		glm::normalize(glm::vec3(-1, -1, -1)),
	};

	bool  tirada_en_curso = true;
	bool  fase_asentado = false;

	glm::quat orientacion_actual = glm::quat(1, 0, 0, 0); // identidad
	glm::quat objetivo_quat = glm::quat(1, 0, 0, 0);

	int   cara_objetivo = 0;     // 0..7
	float tiempo_tirada = 0.0f;

	// Parámetros de animación
	float tiempo_vuelo = 0.8f;  // cuánto dura la voltereta antes de tocar suelo
	float tiempo_asentar = 0.5f;  // tiempo para la cara final

	// Giro libre durante el vuelo
	glm::vec3 eje_giro_libre = glm::normalize(glm::vec3(1.0f, 1.3f, 0.7f));
	float     vel_giro_libre_rad_s = glm::radians(25.0f); // 2 vueltas/seg

	// Caída vertical simple
	float posY_octa = 15.0f;   // altura inicial
	float velY_octa = 0.0f;
	float gravedad = -0.005f;  // velocidad a la que cae


	

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); //DELTA TIME
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 10.0f * deltaTime;
		//dragonavance
		//dragonavance -= 0.1f * deltaTime;



		//ANIMACION SIMPLE DEL COCHE
		/*
		if (movCoche > -250.0f) //> -300.0f por es escalado, >-250 por el translate inicial del carro, <30.0f como venia
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
			rotllanta += rotllantaOffset * deltaTime;
		}*/
		//Animacion en loop
		//Se ejecuta de forma continua mientras la aplicación está activa
		//Si la animacion no es el loop, y se puede inicar varias veces, es estado inicial y el estado final
		//Deben ser el mismo, o agregar elementos para que no se vea que los modelos desaparecen o aparecen de la nada

		//carro
		if (avanza) {
			if (movCoche > -250.0f) {
				movCoche -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else {
				avanza = !avanza;
			}
		}
		else {
			if (movCoche < 325.0f) {
				movCoche += movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;
			}
			else {
				avanza = !avanza;
			}
		}

		//dragon avance
		if (dragonAvanza) {
			if (dragonavance < 20.0f) {
				dragonavance += VelocidadDragon * deltaTime;
				if (dragonavance > 20.0f) dragonavance = 20.0f;
			}
			else {
				dragonAvanza = false; // llego a 20, ahora regresa
			}
		}
		else {
			if (dragonavance > 0.0f) {
				dragonavance -= VelocidadDragon * deltaTime;
				if (dragonavance < 0.0f) dragonavance = 0.0f;
			}
			else {
				dragonAvanza = true; // llego a 0, vuelve a avanzar
			}
		}

		//aleteo
		if (flapSube) {
			flapAngulo += flapVelocidadSubir * deltaTime;
			if (flapAngulo >= flapMax) {
				flapAngulo = flapMax;
				flapSube = false;
			}
		}
		else {
			flapAngulo -= flapVelocidadBajar * deltaTime;
			if (flapAngulo <= flapMin) { 
				flapAngulo = flapMin; 
				flapSube = true;
			}
		}



		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// mov de puertas desde Window (tecla F)
		bool estado_tecla_f = mainWindow.getMuevePuertas(); // true/false
		if (estado_tecla_f != tecla_f_estado_anterior) {
			tecla_f_estado_anterior = estado_tecla_f;
			objetivo_puertas_abiertas = estado_tecla_f; // si F activo -> abrir; si no cerrar
			puertas_en_animacion = true;           // inicia la animacion hacia el objetivo
		}


		if (mainWindow.tomarTiradaSolicitada() && !tirada_en_curso) {
			tirada_en_curso = true;
			fase_asentado = false;
			tiempo_tirada = 0.0f;

			// Reset caída y orientación
			posY_octa = 20.0f;
			velY_octa = 0.0f;
			orientacion_actual = glm::quat(1, 0, 0, 0); // identidad

			// Randomiza el eje de la voltereta
			eje_giro_libre = glm::normalize(glm::vec3(
				0.5f + (rand() % 150) / 100.0f,
				0.7f + (rand() % 150) / 100.0f,
				0.3f + (rand() % 150) / 100.0f
			));
			vel_giro_libre_rad_s = glm::radians(25.0f);   // 25
		}


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();



		if (tirada_en_curso) {
			tiempo_tirada += deltaTime;

			if (!fase_asentado) {
				// ===== FASE 1: caída con volteretas =====
				// Física vertical
				velY_octa += gravedad * deltaTime;
				posY_octa += velY_octa * deltaTime;

				// Giro libre continuo
				float ang_extra = vel_giro_libre_rad_s * deltaTime;
				glm::quat q_spin = glm::angleAxis(ang_extra, glm::normalize(eje_giro_libre));
				orientacion_actual = q_spin * orientacion_actual;

				// si ya toco el piso
				if (posY_octa <= -1.0f) {
					posY_octa = -1.0f;
					velY_octa = 0.0f;

					// Elegir cara aleatoria
					cara_objetivo = rand() % 8;

					// Normal de esa cara en MUNDO = R_actual * n_modelo
					glm::vec3 n_model = normales_octa[cara_objetivo];
					glm::vec3 n_mundo = glm::normalize(glm::mat3_cast(orientacion_actual) * n_model);

					// Rotación de corrección que lleva n_mundo  +Y
					glm::vec3 up(0.0f, 1.0f, 0.0f);
					float d = glm::clamp(glm::dot(n_mundo, up), -1.0f, 1.0f);
					float ang = acosf(d);

					glm::vec3 eje;
					if (fabsf(d - 1.0f) < 1e-4f) {
						eje = glm::vec3(1, 0, 0);
						ang = 0.0f;
					}
					else if (fabsf(d + 1.0f) < 1e-4f) {
						eje = glm::normalize(glm::cross(n_mundo, glm::vec3(1, 0, 0)));
						if (glm::length(eje) < 1e-4f) eje = glm::normalize(glm::cross(n_mundo, glm::vec3(0, 0, 1)));
						ang = glm::pi<float>();
					}
					else {
						eje = glm::normalize(glm::cross(n_mundo, up));
					}

					// rota desde la orientación ACTUAL hacia la deseada
					glm::quat q_fix = glm::angleAxis(ang, eje);
					// El objetivo final es aplicar esa corrección sobre la orientación actual
					objetivo_quat = q_fix * orientacion_actual;

					// Inicia fase de asentado
					fase_asentado = true;
					tiempo_tirada = 0.0f; // reutilizamos como reloj del asentado
				}
			}
			else {
				// ===== FASE 2: asentado =====
				float t = glm::clamp(tiempo_tirada / tiempo_asentar, 0.0f, 1.0f);
				orientacion_actual = glm::slerp(orientacion_actual, objetivo_quat, t);

				if (t >= 1.0f - 1e-4f) {
					orientacion_actual = objetivo_quat; // clava exacto
					tirada_en_curso = false;            // fin
				}
			}
		}


		//INSTANCIA DEL OCTAEDRO
		modelOct = glm::mat4(1.0f);
		modelOct = glm::translate(modelOct, glm::vec3(0.0f, posY_octa, -30.0f));
		modelOct *= glm::mat4_cast(orientacion_actual); // aplicar orientación
		modelOct = glm::scale(modelOct, glm::vec3(3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelOct));
		octaedroTexture.UseTexture();
		meshList[8]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche - 50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//===============DRAGON==================
		//dragonavance para moverlo hacia adelante y atrás en X
		//Cuerpo del dragón
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f - dragonavance, 5.0f + sin(glm::radians(angulovaria)), 15.0));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoDragon_M.RenderModel();

		//Ala izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, 0.23f, 3.6f)); //(x,z,y)
		model = glm::rotate(model, glm::radians(flapAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaIzq_M.RenderModel();

		//Ala derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, 0.17f, 3.57f)); //(x,z,y)
		model = glm::rotate(model, glm::radians(-flapAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaDer_M.RenderModel();
		//=======================================


		// ===== LÓGICA DE ANIMACIÓN DE LAS PUERTAS =====
		if (puertas_en_animacion) {

			if (objetivo_puertas_abiertas) {
				// ABRIR
				// Puerta 1: rotar hasta alcanzar 90 grados
				angulo_puerta1 += velocidad_puerta1_grados * deltaTime;
				if (angulo_puerta1 > 90.0f) angulo_puerta1 = 90.0f;

				// Puerta 2: primero se desplaza en +Z hasta 2.0, luego en +X hasta 4.0
				if (desplazamiento_puerta2_z < 2.0f) {
					desplazamiento_puerta2_z += velocidad_puerta2_z * deltaTime;
					if (desplazamiento_puerta2_z > 2.0f) desplazamiento_puerta2_z = 2.0f;
				}
				else {
					desplazamiento_puerta2_x += velocidad_puerta2_x * deltaTime;
					if (desplazamiento_puerta2_x > 4.0f) desplazamiento_puerta2_x = 4.0f;
				}

				// Termina la animación cuando ambas puertas alcanzan su posición final
				if (angulo_puerta1 >= 90.0f && desplazamiento_puerta2_z >= 2.0f && desplazamiento_puerta2_x >= 4.0f) {
					puertas_en_animacion = false;
				}

			}
			else {
				// CERRAR
				// Puerta 1: regresa a 0 grados
				angulo_puerta1 -= velocidad_puerta1_grados * deltaTime;
				if (angulo_puerta1 < 0.0f) angulo_puerta1 = 0.0f;

				// Puerta 2: primero regresa en X a 0, luego en Z a 0 (orden inverso de apertura)
				if (desplazamiento_puerta2_x > 0.0f) {
					desplazamiento_puerta2_x -= velocidad_puerta2_x * deltaTime;
					if (desplazamiento_puerta2_x < 0.0f) desplazamiento_puerta2_x = 0.0f;
				}
				else {
					desplazamiento_puerta2_z -= velocidad_puerta2_z * deltaTime;
					if (desplazamiento_puerta2_z < 0.0f) desplazamiento_puerta2_z = 0.0f;
				}

				// Termina cuando ambas puertas regresan a su posición inicial
				if (angulo_puerta1 <= 0.0f && desplazamiento_puerta2_z <= 0.0f && desplazamiento_puerta2_x <= 0.0f) {
					puertas_en_animacion = false;
				}
			}
		}




		//===============ARCO CON LETREO Y PUERTAS==================
		//Arco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco_M.RenderModel();

		// puerta 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 2.5f)); // (+z, +x, +y)
		// Rotación de apertura y cierre.
		model = glm::rotate(model, -angulo_puerta1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta1_M.RenderModel();

		// puerta 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 2.5f)); // base (+z, +x, +y)
		// Desplazamiento para abrir y cerrar
		model = glm::translate(model, glm::vec3(desplazamiento_puerta2_z, desplazamiento_puerta2_x, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta2_M.RenderModel();

		//Letrero
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 7.0f));	//(+z, +x, +y)
		modelLetras = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		// ===== MARQUESINA DE 3 LETRAS =====
		avance_celda += velocidad_marquesina * deltaTime;
		while (avance_celda >= 1.0f) {
			avance_celda -= 1.0f;
			indice_central = (indice_central + 1) % CANT_CARACTERES;
		}

		// 1) Índices basados en la letra central
		idxIzq = (indice_central + CANT_CARACTERES - 1) % CANT_CARACTERES;
		idxCentro = indice_central;
		idxDer = (indice_central + 1) % CANT_CARACTERES;

		// 2)Posiciones en X usando -avance  para desplazar a la izquierda
		posCentro = -avance_celda * separacion_letras;
		posIzq = posCentro - separacion_letras;
		posDer = posCentro + separacion_letras;

		// Activar textura
		LetrasPTexture.UseTexture();

		// Letra izquierda
		if (idxIzq != IDX_ESPACIO1 && idxIzq != IDX_ESPACIO2) {
			glm::vec2 toffset(u_letras[idxIzq], v_letras[idxIzq]);
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

			modelL = modelLetras;
			modelL = glm::translate(modelL, glm::vec3(baseZ, desplazamientoX_inicial + posIzq, baseY));
			modelL = glm::scale(modelL, escala_letra);
			modelL = glm::rotate(modelL, rotZ_letra, glm::vec3(0.0f, 0.0f, 1.0f));
			modelL = glm::rotate(modelL, rotY_letra, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelL));
			meshList[7]->RenderMesh();
		}

		// Letra centro
		if (idxCentro != IDX_ESPACIO1 && idxCentro != IDX_ESPACIO2) {
			glm::vec2 toffset(u_letras[idxCentro], v_letras[idxCentro]);
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

			modelC = modelLetras;
			modelC = glm::translate(modelC, glm::vec3(baseZ, desplazamientoX_inicial + posCentro, baseY));
			modelC = glm::scale(modelC, escala_letra);
			modelC = glm::rotate(modelC, rotZ_letra, glm::vec3(0.0f, 0.0f, 1.0f));
			modelC = glm::rotate(modelC, rotY_letra, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
			meshList[7]->RenderMesh();
		}

		// Letra derecha
		if (idxDer != IDX_ESPACIO1 && idxDer != IDX_ESPACIO2) {
			glm::vec2 toffset(u_letras[idxDer], v_letras[idxDer]);
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

			modelR = modelLetras;
			modelR = glm::translate(modelR, glm::vec3(baseZ, desplazamientoX_inicial + posDer, baseY));
			modelR = glm::scale(modelR, escala_letra);
			modelR = glm::rotate(modelR, rotZ_letra, glm::vec3(0.0f, 0.0f, 1.0f));
			modelR = glm::rotate(modelR, rotY_letra, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelR));
			meshList[7]->RenderMesh();
		}
		// despues de dibujar las 3 letras del letrero para no afectar otros objetos reseteamos el offset
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f, 0.0f)));


		/* 
		//Imagen textura del letrero, esto no lo tomo en cuenta, solo lo hice para probar si funcionaba bien
		model = modelLetras;
		model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.0f));	//(+z, +x, +y)
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LetrasPTexture.UseTexture();
		meshList[7]->RenderMesh();*/

		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();
		
		for (int i = 1; i < 4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		}

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}


		//número cambiante 
		/*
		EJERCICIO 1
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		tActual = glfwGetTime();// Obtener el tiempo actual

		if (tActual - tiempoAcum_Num >= intervaloCambio) {
			toffsetnumerocambiau += pasoU;
			if (toffsetnumerocambiau > 1.0f)
				toffsetnumerocambiau = 0.0f;

			tiempoAcum_Num = tActual; // reiniciamos tiempoAcum_Num
		}

		toffsetnumerov = 0.00;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();



		//EJERCICIO 2
		//cambiar automáticamente entre textura número 1 y número 2
		double tNow = glfwGetTime();

		if (tNow - tiempoAcum >= intervalo) {
			cambiaNum = !cambiaNum;
			tiempoAcum = tNow;
		}

		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Alternar automáticamente entre las dos texturas
		if (cambiaNum) {
			Numero1Texture.UseTexture();
		}
		else {
			Numero2Texture.UseTexture();
		}
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();




		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
