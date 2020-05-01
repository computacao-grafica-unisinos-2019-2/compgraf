#ifndef SYSTEM_H
#define SYSTEM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

// External Libs
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Group.h"
#include "Obj3D.h"
#include "Obj3DReader.h"
#include "Camera.h"

class System
{
private:
	// Screen
	const GLint WIDTH = 800, HEIGHT = 600;
	int screenWidth, screenHeight;

public:
	GLFWwindow* window;
	Shader coreShader;
    Obj3DReader* obj3DReader;
	std::vector<Obj3D*> objects;

public:
	System();
	~System();

	int GLFWInit();
	int OpenGLSetup();
	int SystemSetup();

	void Run();

	void Finish();

    void reflectBullet();

    void reflectBullet(Obj3D *bullet);

    void processCollisions(int i);

    void shoot(Obj3D *modelObject);

    void moveBullet(Obj3D *bullet);

    void moveBullet(Obj3D *bullet, float deltaTime);
};

#endif