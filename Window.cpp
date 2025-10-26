#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	mueveCofre = 0.0f;
	avanza = false;
	retrocede = false;
	luzLampara = true;    // enciende por defecto
	luzAntorcha = false;  // arranca apagada
	mueveHeli = 0.0f;
	luzPokeEncendida = true; // inicia encendida
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 08: Iluminacion Parte 2", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex -= 1.0;
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			theWindow->avanza = true;
			theWindow->retrocede = false;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->avanza = false;
		}
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex += 1.0;
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			theWindow->retrocede = true;
			theWindow->avanza = false;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->retrocede = false;
		}
	}


	//mover cofre hacia arriba
	if (key == GLFW_KEY_F) {
		theWindow->mueveCofre += 5.0f;
		if (theWindow->mueveCofre > 45.0f) {
			theWindow->mueveCofre = 45.0f; // Limitar a 45 grados
		}
	}
	//mover cofre hacia abajo
	if (key == GLFW_KEY_G) {
		theWindow->mueveCofre -= 5.0f;
		if (theWindow->mueveCofre < 0.0f) {
			theWindow->mueveCofre = 0.0f; // Limitar a 0 grados
		}
	}

	//l�mpara P
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		theWindow->luzLampara = !theWindow->luzLampara;
	}
	//antorcha O
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		theWindow->luzAntorcha = !theWindow->luzAntorcha;
	}



	//Mover helicoptero
	if (key == GLFW_KEY_I) {
		theWindow->mueveHeli += 1.0;
	}
	if (key == GLFW_KEY_O) {
		theWindow->mueveHeli -= 1.0;
	}
	/*
	// Encender/Apagar luz de la Poke Lampara
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		theWindow->luzPokeEncendida = !theWindow->luzPokeEncendida;
	}*/



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
