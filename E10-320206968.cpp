/* 
Animación por keyframes
La textura del skybox fue conseguida desde la página https ://opengameart.org/content/elyvisions-skyboxes?page=1
y edité en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
	https ://www.khronos.org/opengl/wiki/Keyframe_Animation
	http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
	*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS


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

#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cctype>

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
bool avanza;
float angulovaria = 0.0f;

//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

//Variable para el manejo del archivo de keyframes
static const char* ARCHIVO_KEYFRAMES = "keyframes_guardados.txt";
static int volcadoInicialHecho = 0;

//Variables para los movimientos de los keyframes
float cicloXMenos, cicloXMenos2;
float cicloYMas, cicloYMas2;
float cicloYMenos, cicloYMenos2;
float cicloGiro, cicloGiro2;
float cicloCarga, cicloCarga2;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;



Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

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

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;



//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 100 //Número de cuadros máximos
int i_max_steps = 100; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;		//Variable para GiroAvion
	float giroAvionInc;		//Variable para IncrementoGiroAvion
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//El número de cuadros guardados actualmente desde 0 para no sobreescribir
bool play = false;
int playIndex = 0;

// ----------------------------------------------------
// Agrega UN keyframe append, no sobrescribe 
static void AppendKeyframe(const FRAME& fr, int idx,
	const std::string& nombreArchivo = ARCHIVO_KEYFRAMES)
{
	std::ofstream archivo(nombreArchivo, std::ios::app);
	if (!archivo.is_open()) {
		printf("Error al abrir '%s' para guardar el keyframe.\n", nombreArchivo.c_str());
		return;
	}

	archivo << "=========KEYFRAME [" << idx << "]==========\n";
	archivo << std::fixed << std::setprecision(3);
	archivo << "KeyFrame[" << idx << "].movAvion_x = " << fr.movAvion_x << "f;\n";
	archivo << "KeyFrame[" << idx << "].movAvion_y = " << fr.movAvion_y << "f;\n";
	archivo << "KeyFrame[" << idx << "].giroAvion  = " << fr.giroAvion << ";\n";
}

// ----------------------------------------------------
// Guarda TODOS los keyframes [0..FrameIndex-1] en append
void GuardarKeyframesEnArchivo(FRAME KeyFrame[], int FrameIndex, const std::string& nombreArchivo)
{
	std::ofstream archivo(nombreArchivo, std::ios::app); // modo append
	if (!archivo.is_open()) {
		printf("Error al abrir el archivo para guardar los keyframes.\n");
		return;
	}

	archivo << std::fixed << std::setprecision(3);
	for (int i = 0; i < FrameIndex; ++i) {
		archivo << "=========KEYFRAME [" << i << "]==========\n";
		archivo << "KeyFrame[" << i << "].movAvion_x = " << KeyFrame[i].movAvion_x << "f;\n";
		archivo << "KeyFrame[" << i << "].movAvion_y = " << KeyFrame[i].movAvion_y << "f;\n";
		archivo << "KeyFrame[" << i << "].giroAvion  = " << KeyFrame[i].giroAvion << ";\n";
	}

	printf("Se guardaron %d keyframes en '%s'\n", FrameIndex, nombreArchivo.c_str());
}


void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	//Se agregan nuevas líneas para guardar más variables si es necesario
	// agrega solo este keyframe al archivo (append)
	AppendKeyframe(KeyFrame[FrameIndex], FrameIndex, ARCHIVO_KEYFRAMES);
	
	//no volatil,se requiere agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}

static void ImprimirMenuControles()
{
	printf(
		"\n"
		"==============================================================\n"
		"                    CONTROLES DE KEYFRAMES                    \n"
		"==============================================================\n"
		" Animación (keyframes precargados)\n"
		"   [ESPACIO]  Reproducir animación\n"
		"   [0]        Rehabilitar reproducción (desbloquea ESPACIO)\n"
		"\n"
		" Guardado de keyframes  (archivo: %s)\n"
		"   [L]        Guardar keyframes:\n"
		"   [P]        Rehabilitar L para el siguiente guardado\n"
		"\n"
		" Carga de keyframes desde archivo\n"
		"   [M]        Cargar keyframes del archivo\n"
		"   [N]        Rehabilitar M para cargar nuevamente\n"
		"\n"
		" Edición del helicóptero por teclado\n"
		"   [1]  +X     |  [2]  habilitar +X\n"
		"   [3]  -X     |  [4]  habilitar -X\n"
		"   [5]  +Y     |  [6]  habilitar +Y\n"
		"   [7]  -Y     |  [8]  habilitar -Y\n"
		"   [R]  Giro 180  |  [T]  habilitar giro\n"
		"==============================================================\n",
		ARCHIVO_KEYFRAMES
	);
}

static inline std::string trim(const std::string& s) {
	size_t a = 0, b = s.size();
	while (a < b && std::isspace((unsigned char)s[a])) ++a;
	while (b > a && std::isspace((unsigned char)s[b - 1])) --b;
	return s.substr(a, b - a);
}

// Quita sufijos como 'f' y ';' (en cualquier orden), y vuelve a trimear
static inline std::string strip_suffixes(std::string v) {
	v = trim(v);
	while (!v.empty() && (v.back() == ';' || v.back() == 'f' || v.back() == 'F')) v.pop_back();
	return trim(v);
}

// Extrae lo que está a la derecha del '=' y lo convierte a float
static inline bool parse_value_after_equal(const std::string& line, float& out) {
	auto p = line.find('=');
	if (p == std::string::npos) return false;
	std::string val = strip_suffixes(line.substr(p + 1));
	try {
		out = std::stof(val);
		return true;
	}
	catch (...) {
		return false;
	}
}

static bool CargarKeyframesDesdeArchivo(const std::string& nombreArchivo = ARCHIVO_KEYFRAMES)
{
	std::ifstream in(nombreArchivo);
	if (!in.is_open()) {
		printf("ERROR: No se pudo abrir '%s' para leer keyframes.\n", nombreArchivo.c_str());
		return false;
	}

	FRAME tmp[MAX_FRAMES];
	for (int i = 0; i < MAX_FRAMES; ++i) {
		tmp[i].movAvion_x = 0.0f;
		tmp[i].movAvion_y = 0.0f;
		tmp[i].giroAvion = 0.0f;
	}

	std::string line;
	int maxIdx = -1;

	while (std::getline(in, line)) {
		line = trim(line);

		// buscamos encabezado: =========KEYFRAME [i]==========
		if (line.rfind("=========KEYFRAME [", 0) == 0) {
			// encuentra el índice entre [ y ]
			auto lb = line.find('[');
			auto rb = line.find(']', lb == std::string::npos ? 0 : lb + 1);
			if (lb == std::string::npos || rb == std::string::npos || rb <= lb + 1) {
				// encabezado raro, salta
				continue;
			}
			int idx = -1;
			try {
				idx = std::stoi(line.substr(lb + 1, rb - (lb + 1)));
			}
			catch (...) { idx = -1; }

			// leemos 3 líneas siguientes (x, y, giro)
			std::string lx, ly, lg;
			if (!std::getline(in, lx) || !std::getline(in, ly) || !std::getline(in, lg)) {
				printf("ADVERTENCIA: Formato incompleto tras header de keyframe %d.\n", idx);
				break;
			}

			float x = 0.0f, y = 0.0f, g = 0.0f;
			bool okx = parse_value_after_equal(lx, x);
			bool oky = parse_value_after_equal(ly, y);
			bool okg = parse_value_after_equal(lg, g);

			if (idx >= 0 && idx < MAX_FRAMES && okx && oky && okg) {
				tmp[idx].movAvion_x = x;
				tmp[idx].movAvion_y = y;
				tmp[idx].giroAvion = g;
				if (idx > maxIdx) maxIdx = idx;
			}
			else {
				printf("ADVERTENCIA: Keyframe %d con valores no parseables:\n%s\n%s\n%s\n",
					idx, lx.c_str(), ly.c_str(), lg.c_str());
			}
		}
		// si no es header, seguimos
	}
	in.close();

	if (maxIdx < 0) {
		printf("No se encontro ningun keyframe valido en '%s'.\n", nombreArchivo.c_str());
		return false;
	}

	for (int i = 0; i <= maxIdx && i < MAX_FRAMES; ++i) {
		KeyFrame[i] = tmp[i];
	}
	FrameIndex = maxIdx + 1;

	// coloca la aeronave en el primer keyframe cargado
	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;

	// evita que L vuelva a dumpear los iniciales
	volcadoInicialHecho = 1;

	// (opcional) habilita reproducir con ESPACIO sin tocar 0
	reproduciranimacion = 0;
	habilitaranimacion = 0;

	printf("Listo: se cargaron %d keyframes (indices 0..%d) desde '%s'.\n",
		FrameIndex, FrameIndex - 1, nombreArchivo.c_str());
	return true;
}





void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{
				
				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movAvion_x += KeyFrame[playIndex].movAvion_xInc ;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc ;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");

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
		5.0f);
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
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	
	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = -2.0f;
	KeyFrame[1].movAvion_y = 4.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = -4.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = -6.0f;
	KeyFrame[3].movAvion_y = -4.0f;
	KeyFrame[3].giroAvion = 0;


	KeyFrame[4].movAvion_x = -8.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].giroAvion = 0.0f;

	KeyFrame[5].movAvion_x = -10.0f;
	KeyFrame[5].movAvion_y = 4.0f;
	KeyFrame[5].giroAvion = 0.0f;
	
	//Se agregan nuevos frames 


	ImprimirMenuControles();



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		glm::vec3 lowerLight = glm::vec3(0.0f,0.0f,0.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;

		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
		}
		rotllanta += rotllantaOffset * deltaTime;


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

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


		model=glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
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


		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
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
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		model = glm::mat4(1.0);
		posblackhawk=glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();
		
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1) //Solo quiero entrar una vez
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproduciranimacion = 0;
			
		}
	}

	/*
	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}*/

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			// 1) Primera vez: volcar los existentes [0..FrameIndex-1]
			if (!volcadoInicialHecho) {
				GuardarKeyframesEnArchivo(KeyFrame, FrameIndex, ARCHIVO_KEYFRAMES);
				volcadoInicialHecho = 1;
				printf("Se guardaron los keyframes iniciales [0..%d] en '%s'\n", FrameIndex - 1, ARCHIVO_KEYFRAMES);
			}
			// 2) Siguientes veces: crear y guardar SOLO el nuevo
			else {
				saveFrame();  // adentro ya llama a AppendKeyframe(...)
				printf("Se guardo el nuevo keyframe en '%s'\n", ARCHIVO_KEYFRAMES);
			}

			printf("Presiona P para habilitar guardar otro frame.\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}



	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
			reinicioFrame++;
		}
	}

	//MOVIMIENTO EN +X DE LA AERONAVE CON TECLAS 1 Y 2
	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			ciclo2++;
			printf("\n Ya puedes modificar tu variable presionando la tecla 1\n");
		}
	}

	//Movimiento en -X de la aeronave con teclas 3 y 4
	if (keys[GLFW_KEY_3])
	{
		if (cicloXMenos < 1)
		{
			movAvion_x -= 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			cicloXMenos++;
			cicloXMenos2 = 0;
			printf("\n Presiona la tecla 4 para poder habilitar la variable\n");
		}
	}
	if (keys[GLFW_KEY_4])
	{
		if (cicloXMenos2 < 1)
		{
			cicloXMenos = 0;
			cicloXMenos2++;
			printf("\n Ya puedes modificar tu variable presionando la tecla 3\n");
		}
	}

	//Movimiento en +Y de la aeronave con teclas 5 y 6
	if (keys[GLFW_KEY_5])
	{
		if (cicloYMas < 1)
		{
			movAvion_y += 1.0f;
			printf("\n movAvion_y es: %f\n", movAvion_y);
			cicloYMas++;
			cicloYMas2 = 0;
			printf("\n Presiona la tecla 6 para poder habilitar la variable\n");
		}
	}
	if (keys[GLFW_KEY_6])
	{
		if (cicloYMas2 < 1)
		{
			cicloYMas = 0;
			cicloYMas2++;
			printf("\n Ya puedes modificar tu variable presionando la tecla 5\n");
		}
	}

	//Movimiento en -Y de la aeronave con teclas 7 y 8
	if (keys[GLFW_KEY_7])
	{
		if (cicloYMenos < 1)
		{
			movAvion_y -= 1.0f;
			printf("\n movAvion_y es: %f\n", movAvion_y);
			cicloYMenos++;
			cicloYMenos2 = 0;
			printf("\n Presiona la tecla 8 para poder habilitar la variable\n");
		}
	}
	if (keys[GLFW_KEY_8])
	{
		if (cicloYMenos2 < 1)
		{
			cicloYMenos = 0;
			cicloYMenos2++;
			printf("\n Ya puedes modificar tu variable presionando la tecla 7\n");
		}
	}


	//Giro de 180 grados con teclas R y T
	if (keys[GLFW_KEY_R])
	{
		if (cicloGiro < 1)
		{
			giroAvion += 180.0f;
			if (giroAvion >= 360.0f){
				giroAvion = giroAvion - 360.0f;
			}

			printf("\n giroAvion es: %f \n", giroAvion);
			cicloGiro++;
			cicloGiro2 = 0;
			printf("\n Presiona la tecla T para poder habilitar el giro nuevamente\n");
		}
	}
	if (keys[GLFW_KEY_T])
	{
		if (cicloGiro2 < 1)
		{
			cicloGiro = 0;
			cicloGiro2++;
			printf("\n Ya puedes aplicar nuevamente el giro con la tecla R\n");
		}
	}

	// CARGAR KEYFRAMES DESDE ARCHIVO M y habilitar N
	if (keys[GLFW_KEY_M]) {
		if (cicloCarga < 1) {
			if (CargarKeyframesDesdeArchivo(ARCHIVO_KEYFRAMES)) {
				printf("\nCARGA COMPLETA: KeyFrame[0] -> x=%.3f, y=%.3f, giro=%.3f\n",
					KeyFrame[0].movAvion_x, KeyFrame[0].movAvion_y, KeyFrame[0].giroAvion);
			}
			else {
				printf("\nNo se pudieron cargar keyframes. Verifica el archivo '%s'.\n", ARCHIVO_KEYFRAMES);
			}
			cicloCarga++;
			cicloCarga2 = 0;
			printf("\nPresiona la tecla N para habilitar cargar nuevamente.\n");
		}
	}
	if (keys[GLFW_KEY_N]) { 
		if (cicloCarga2 < 1) {
			cicloCarga = 0;
			cicloCarga2++;
			printf("\nYa puedes volver a cargar keyframes con la tecla M.\n");
		}
	}






}
