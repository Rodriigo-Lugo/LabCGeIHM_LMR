#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmueveHeli() { return mueveHeli; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	bool getLuzPokeEncendida() const { return luzPokeEncendida; }
	GLboolean getCocheAvanza() { return avanza; }
	GLboolean getCocheRetrocede() { return retrocede; }
	GLboolean getLuzLampara() { return luzLampara; }
	GLboolean getLuzAntorcha() { return luzAntorcha; }
	GLfloat getmueveCofre() { return mueveCofre; }

	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat mueveHeli;
	GLfloat mueveCofre;
	GLboolean avanza;
	GLboolean retrocede;
	bool luzPokeEncendida;
	bool mouseFirstMoved;
	bool luzLampara;
	bool luzAntorcha;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

