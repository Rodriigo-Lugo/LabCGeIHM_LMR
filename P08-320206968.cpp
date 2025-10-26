/*
Práctica 7: Iluminación 1 
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaedroTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model BaseCarro_M;
Model Llanta_AdelanteD_M;
Model Llanta_AdelanteI_M;
Model Llanta_AtrasD_M;
Model Llanta_AtrasI_M;
Model OjosCarro_M;
Model CofreCarro_M;
Model ParrillaCarro_M;
Model PokeLampara_M;
Model Pared_M;
Model Antorcha_M;


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
// para declarar varias luces de tipo spotlight
SpotLight spotLights[4];

//para declarar varias luces de tipo pointlight
PointLight pointLights[3];

//Segundo Arreglo de luces puntuales
PointLight pointLights2[3];//MAX_POINT_LIGHTS es el mismo o diferente?

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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
	unsigned int indices[] = {
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

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
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




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearOctaedro();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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
	octaedroTexture = Texture("Textures/octaedro_numeros_gimp.png");
	octaedroTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	BaseCarro_M = Model();
	BaseCarro_M.LoadModel("Models/BaseCarro.fbx");
	Llanta_AdelanteD_M = Model();
	Llanta_AdelanteD_M.LoadModel("Models/LlantaAdelanteD.fbx");
	Llanta_AdelanteI_M = Model();
	Llanta_AdelanteI_M.LoadModel("Models/LlantaAdelanteI.fbx");
	Llanta_AtrasD_M = Model();
	Llanta_AtrasD_M.LoadModel("Models/LlantaAtrasD.fbx");
	Llanta_AtrasI_M = Model();
	Llanta_AtrasI_M.LoadModel("Models/LlantaAtrasI.fbx");
	OjosCarro_M = Model();
	OjosCarro_M.LoadModel("Models/OjosCarro.obj");
	CofreCarro_M = Model();
	CofreCarro_M.LoadModel("Models/CofreCarro.obj");
	ParrillaCarro_M = Model();
	ParrillaCarro_M.LoadModel("Models/ParrillaCarro.obj");
	PokeLampara_M = Model();
	PokeLampara_M.LoadModel("Models/PokeLampara.obj");
	Pared_M = Model();
	Pared_M.LoadModel("Models/Pared.obj");
	Antorcha_M = Model();
	Antorcha_M.LoadModel("Models/Antorcha.obj");

	

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
	// Luz blanca para pokebola
	pointLights[0] = PointLight(
		1.0f, 1.0f, 1.0f,   // color blanco
		1.0f, 1.5f,        // IA , ID 
		0.0f, 4.0f, -10.0f,  // posicion
		0.5f, 0.01f, 0.01f // atenuación
	);
	pointLightCount++;

	//antorcha
	pointLights[1] = PointLight(
		1.0f, 0.521f, 0.019f,   // color blanco
		1.0f, 1.5f,        // IA , ID 
		0.0f, 4.0f, -10.0f,  // posicion
		0.5f, 0.01f, 0.01f // atenuación
	);
	pointLightCount++;
	
	
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, //blanca
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// luz de cofre del auto
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,   //verde
		1.0f, 2.0f,         // IA, ID
		0.0f, 0.0f, 0.0f,   // pos
		1.0f, 0.0f, 0.0f,  // dir 
		1.0f, 0.0, 0.0, // atenuacion
		10.0f               // angulo apertura 
	);
	spotLightCount++;
	
	// luz delantera del auto
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,   //azul
		1.0f, 2.0f,         // IA, ID
		0.0f, 0.0f, 0.0f,   // pos
		1.0f, 0.0f, 0.0f,  // dir 
		1.0f, 0.0, 0.0, // atenuacion
		15.0f               // angulo apertura 
	);
	spotLightCount++;

	// luz trasera del auto
	spotLights[3] = SpotLight(1.0f, 0.0f, 0.0f,   //roja
		1.0f, 2.0f,         // IA, ID
		0.0f, 0.0f, 0.0f,   // pos
		1.0f, 0.0f, 0.0f,  // dir 
		1.0f, 0.0, 0.0, // atenuacion
		15.0f               // angulo apertura 
	);
	spotLightCount++;

	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//===CREACION DE VARIABLES FUERA DEL WHILE PARA OPTIMIZAR EL CODIGO===//
	// Spotlights
	int spotActivos = 0;

	//pointLights
	unsigned int pointActivos = 0;

	// LUZ DELANTERA AZUL
	glm::vec3 DirFaroLocal = glm::vec3(0.0f, 0.0f, 0.0f); // dirección local
	glm::vec3 PosFaroLocal = glm::vec3(0.0f, 0.0f, 0.0f); // posicion local 
	glm::vec3 PosFaroGlobal = glm::vec3(0.0f, 0.0f, 0.0f); //posición global
	glm::vec3 DirFaroGlobal = glm::vec3(0.0f, 0.0f, 0.0f); //dirección global

	// LUZ TRASERA ROJA
	glm::vec3 DirFaroTLocal = glm::vec3(0.0f, 0.0f, 0.0f); // dirección local
	glm::vec3 PosFaroTLocal = glm::vec3(0.0f, 0.0f, 0.0f); // posicion local
	glm::vec3 PosFaroTGlobal = glm::vec3(0.0f, 0.0f, 0.0f); //posición global
	glm::vec3 DirFaroTGlobal = glm::vec3(0.0f, 0.0f, 0.0f); //dirección global

	// LUZ COFRE
	glm::vec3 DirCofreLocal = glm::vec3(0.0f, 0.0f, 0.0f); // dirección local
	glm::vec3 PosCofreLocal = glm::vec3(0.0f, 0.0f, 0.0f); // posicion local 
	glm::vec3 PosCofreGlobal = glm::vec3(0.0f, 0.0f, 0.0f); //posición global
	glm::vec3 DirCofreGlobal = glm::vec3(0.0f, 0.0f, 0.0f); //dirección global

	//LUZ POKELAMPARA
	glm::vec3 posLuzPokeLocal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 posLuzPokeGlobal = glm::vec3(0.0f, 0.0f, 0.0f);

	//ANTORCHA
	glm::vec3 posLuzAntorchaLocal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 posLuzAntorchaGlobal = glm::vec3(0.0f, 0.0f, 0.0f);

	//pivote cofre
	glm::vec3 pivoteLocalOBJ = glm::vec3(0.0f, 1.2f, -3.0f);

	//matrices modelos
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelauxLuz(1.0);
	glm::mat4 modelauxCofre(1.0);
	glm::mat4 modelAntorcha(1.0);
	glm::mat4 modelPokeLampara(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//==================SPOTLIGHTS ACTIVAS=========================
		// Arreglo Spotlight de luces activas seria linterna y cofre siempre, y delantera o trasera segun sea el caso
		SpotLight lucesSpotActivas[3];//3: linterna, cofre y una de movimiento
		spotActivos = 0;

		// Siempre encendidas
		lucesSpotActivas[spotActivos++] = spotLights[0]; // linterna ligada a cámara
		lucesSpotActivas[spotActivos++] = spotLights[1]; // cofre

		// Segun el movimiento con teclas Y/U y las banderas agregadas en window.cpp y window.h
		if (mainWindow.getCocheAvanza()) {
			lucesSpotActivas[spotActivos++] = spotLights[2]; // delantera
		}
		else if (mainWindow.getCocheRetrocede()) {
			lucesSpotActivas[spotActivos++] = spotLights[3]; // trasera
		}
		//unica llamada de las spotlights
		shaderList[0].SetSpotLights(lucesSpotActivas, spotActivos);

		//================POINTLIGHTS ACTIVAS=========================
		//arreglo con sólo las point lights encendidas
		PointLight lucesPuntoActivas[2];
		pointActivos = 0;

		if (mainWindow.getLuzLampara()) {
			lucesPuntoActivas[pointActivos++] = pointLights[0]; // lampara luz blanca
		}
		if (mainWindow.getLuzAntorcha()) {
			lucesPuntoActivas[pointActivos++] = pointLights[1]; // antorcha luz naranja
		}
		// unica llamada de las pointliggts
		shaderList[0].SetPointLights(lucesPuntoActivas, pointActivos);

		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//================AUTO==========================
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), -0.5f, -3.0f));
		modelaux = model;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxLuz = model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);//color azul para el coche
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		BaseCarro_M.RenderModel();
		
		// LUZ DELANTERA AZUL
		DirFaroLocal = glm::vec3(1.0f, 0.0f, 0.0f); // dirección local
		PosFaroLocal = glm::vec3(3.0f, 0.0f, 0.5f); // posicion local 
		PosFaroGlobal = glm::vec3(modelauxLuz * glm::vec4(PosFaroLocal, 1.0f)); //posición global
		DirFaroGlobal = glm::normalize(glm::vec3(modelauxLuz * glm::vec4(DirFaroLocal, 0.0f))); //dirección global
		spotLights[2].SetFlash(PosFaroGlobal, DirFaroGlobal);

		// LUZ TRASERA ROJA
		DirFaroTLocal = glm::vec3(-1.0f, 0.0f, 0.0f); // dirección local
		PosFaroTLocal = glm::vec3(-3.0f, 0.0f, 1.3f); // posicion local
		PosFaroTGlobal = glm::vec3(modelauxLuz * glm::vec4(PosFaroTLocal, 1.0f)); //posición global
		DirFaroTGlobal = glm::normalize(glm::vec3(modelauxLuz * glm::vec4(DirFaroTLocal, 0.0f))); //dirección global
		spotLights[3].SetFlash(PosFaroTGlobal, DirFaroTGlobal);


		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.6f, 0.4f, 1.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_AdelanteI_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.6f, 0.4f, 1.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_AtrasI_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.6f, 0.4f, -1.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_AdelanteD_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.6f, 0.4f, -1.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_AtrasD_M.RenderModel();

		//Ojos del coche
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.0f, -0.2f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		OjosCarro_M.RenderModel();
		
		//Cofre del coche
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.0f, -0.15f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, pivoteLocalOBJ);          // mover al pivote
		model = glm::rotate(model, glm::radians(mainWindow.getmueveCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -pivoteLocalOBJ);          // regresar del pivote
		modelauxLuz = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CofreCarro_M.RenderModel();

		// LUZ COFRE
		DirCofreLocal = glm::vec3(0.0f, 0.0f, -1.0f); // dirección local
		PosCofreLocal = glm::vec3(0.0f, 0.5f, -3.0f); // posicion local 

		PosCofreGlobal = glm::vec3(modelauxLuz * glm::vec4(PosCofreLocal, 1.0f)); //posición global
		DirCofreGlobal = glm::normalize(glm::vec3(modelauxLuz * glm::vec4(DirCofreLocal, 0.0f))); //dirección global

		spotLights[1].SetFlash(PosCofreGlobal, DirCofreGlobal);

		//Parrilla del coche
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.0f, -0.15f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ParrillaCarro_M.RenderModel();
		// ======================================================================

		//===============PARED==========================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 19.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pared_M.RenderModel();
		//=============================================

		//POKE LAMPARA
		modelPokeLampara = glm::mat4(1.0);
		modelPokeLampara = glm::translate(modelPokeLampara, glm::vec3(-10.0f, -1.0f, -30.0f));
		modelPokeLampara = glm::rotate(modelPokeLampara, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelPokeLampara = glm::scale(modelPokeLampara, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPokeLampara));
		PokeLampara_M.RenderModel();
		// JERARQUIA
		posLuzPokeLocal = glm::vec3(0.0f, 4.0f, 0.0f);
		posLuzPokeGlobal = glm::vec3(modelPokeLampara * glm::vec4(posLuzPokeLocal, 1.0f));
		// Actualiza la posición (jerarquía)
		pointLights[0].SetPos(posLuzPokeGlobal);//SetPos agregado en PointLight.h
		//=============================================

		//===============ANTORCHA==========================
		modelAntorcha = glm::mat4(1.0);
		modelAntorcha = glm::translate(modelAntorcha, glm::vec3(20.0f, -0.9f, -30.0f));
		modelAntorcha = glm::scale(modelAntorcha, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAntorcha));
		Antorcha_M.RenderModel();
		// JERARQUIA
		posLuzAntorchaLocal = glm::vec3(0.0f, 3.0f, 0.0f);
		posLuzAntorchaGlobal = glm::vec3(modelAntorcha * glm::vec4(posLuzAntorchaLocal, 1.0f));
		// Actualiza la posición (jerarquía)
		pointLights[1].SetPos(posLuzAntorchaGlobal);//SetPos agregado en PointLight.h



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
