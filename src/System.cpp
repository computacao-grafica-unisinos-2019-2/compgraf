#include "System.h"
#include "Shader.cpp"

bool firstMouse = true;
float mouseX = 800 / 2.0f;
float mouseY = 600 / 2.0f;
Camera * camera = new Camera();

void onScroll(GLFWwindow *window, double xPosition, double yPosition) {
    if (firstMouse) {
        mouseX = xPosition;
        mouseY = yPosition;
        firstMouse = false;
    }

    float offsetX = xPosition - mouseX;
    float offsetY = mouseY - yPosition;

    mouseX = xPosition;
    mouseY = yPosition;

    camera->processMouseInput(offsetX, offsetY);
}

System::System()
{
}


System::~System()
{
}

int System::GLFWInit()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	glfwWindowHint( GLFW_SAMPLES, 4 );

	window = glfwCreateWindow( WIDTH, HEIGHT, "Call of Duty", nullptr, nullptr );

	glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
	glfwSetCursorPosCallback(window, onScroll);

	if ( window == nullptr ) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );

	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK ) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport( 0, 0, screenWidth, screenHeight );

	obj3DReader = new Obj3DReader();

	return EXIT_SUCCESS;

}

int System::OpenGLSetup()
{
	glEnable( GL_BLEND );	// Enables blending ( glBlendFunc )
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_TEXTURE_2D);

	glEnable( GL_DEPTH_TEST );

//	glEnable( GL_CULL_FACE );
//	glCullFace( GL_BACK );
//	glFrontFace( GL_CW );

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	coreShader = Shader( "resources/shaders/core/core.vert", "resources/shaders/core/core.frag" );
    coreShader.Use();

	return EXIT_SUCCESS;
}

void System::Run()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    coreShader.Use();
    coreShader.LoadTexture( "resources/textures/bricks.jpg", "texture1", "bricksTexture" );

    Obj3D *cube = obj3DReader->readObjFromFile("/Users/i848340/unisinos/compgraf/resources/objects/cubo.obj");
    cube->destroyed = true;
    objects.push_back(cube);

    Obj3D *sphere = obj3DReader->readObjFromFile("/Users/i848340/unisinos/compgraf/resources/objects/sphere.obj");
    sphere->scale = 0.5f;
    sphere->position = new glm::vec3(-8.0f, -5.0f, -15.0f);
    objects.push_back(sphere);

    Obj3D *obj1 = new Obj3D();
    obj1->mesh = sphere->mesh;
    obj1->min = sphere->min;
    obj1->max = sphere->max;
    obj1->scale = 0.5f;
    obj1->canBeDestroyed = false;
    obj1->position = new glm::vec3(0.0f, -5.0f, -15.0f);
    objects.push_back(obj1);

    Obj3D *obj2 = new Obj3D();
    obj2->mesh = sphere->mesh;
    obj2->min = sphere->min;
    obj2->max = sphere->max;
    obj2->scale = 0.5f;
    obj2->position = new glm::vec3(8.0f, -5.0f, -15.0f);
    objects.push_back(obj2);

    Obj3D *obj3 = new Obj3D();
    obj3->mesh = sphere->mesh;
    obj3->min = sphere->min;
    obj3->max = sphere->max;
    obj3->scale = 0.5f;
    obj3->canBeDestroyed = false;
    obj3->position = new glm::vec3(16.0f, -5.0f, -15.0f);
    objects.push_back(obj3);

    std::vector<float> finalVector;

    for (int l = 0; l < objects.size(); ++l) {

        Mesh* mesh = objects[l]->mesh;
        Group* group;
        Face* face;
        glm::vec3* vertex;
        glm::vec3* normal;
        glm::vec2* texture;

        for (int i = 0; i < mesh->groups.size(); ++i) {
            group = mesh->groups[i];
            for (int j = 0; j < group->faces.size(); ++j) {
                face = group->faces[j];

                for (int k = 0; k < face->verticesCount(); ++k) {
                    vertex = mesh->vertices[face->vertices[k]];
                    finalVector.push_back(vertex->x);
                    finalVector.push_back(vertex->y);
                    finalVector.push_back(vertex->z);

                    texture = mesh->mappings[face->textures[k]];
                    finalVector.push_back(texture->x);
                    finalVector.push_back(texture->y);

                    normal = mesh->normals[face->normals[k]];
                    finalVector.push_back(normal->x);
                    finalVector.push_back(normal->y);
                    finalVector.push_back(normal->z);
                }
            }

            GLuint VBO;

            glGenVertexArrays(1, &group->VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(group->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData( GL_ARRAY_BUFFER, group->numOfVertices * sizeof(GLfloat), finalVector.data(), GL_STATIC_DRAW );

            // vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0));
            glEnableVertexAttribArray( 0 );

            // textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), ( GLvoid * ) ( 3 * sizeof( GLfloat ) ));
            glEnableVertexAttribArray( 1 );

            // normals
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), ( GLvoid * ) ( 5 * sizeof( GLfloat ) ));
            glEnableVertexAttribArray( 2 );
        }
    }

	while ( !glfwWindowShouldClose( window ) ) {

        float currentSeconds = glfwGetTime();
        deltaTime = currentSeconds - lastFrame;
        lastFrame = currentSeconds;
		glfwPollEvents();

#pragma region Input Handling

		if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
			glfwSetWindowShouldClose( window, GLFW_TRUE );
		}

		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            camera->processKeyboardInput(FORWARD,deltaTime);
		}

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            camera->processKeyboardInput(RIGHT,deltaTime);
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            camera->processKeyboardInput(LEFT,deltaTime);
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            camera->processKeyboardInput(BACKWARD,deltaTime);
        }

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            shoot(obj3);
        }


#pragma endregion

		glClearColor( 0.2f, 0.3f, 0.8f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		coreShader.Use();

		coreShader.UseTexture( "bricksTexture" );

        for (int i = 0; i < objects.size(); ++i) {
            if (objects[i]->isProjectile){
                moveBullet(objects[i], deltaTime);
                processCollisions(i);
            }

            objects[i]->draw(camera, &coreShader);
        }

		glfwSwapBuffers( window );
	}
}

void System::shoot(Obj3D *modelObject) {
    Obj3D *bullet = new Obj3D();
    bullet->mesh = modelObject->mesh;
    bullet->min = modelObject->min;
    bullet->max = modelObject->max;
    bullet->isProjectile = true;
    bullet->destroyed = false;
    bullet->scale = 0.01f;
    bullet->direction = new glm::vec3(camera->front);
    bullet->position = new glm::vec3(camera->position + camera->front);
    objects.push_back(bullet);
}

void System::moveBullet(Obj3D *bullet, float deltaTime) {
    bullet->position->x = bullet->position->x + (deltaTime * 5 * bullet->direction->x);
    bullet->position->y = bullet->position->y + (deltaTime * 5 * bullet->direction->y);
    bullet->position->z = bullet->position->z + (deltaTime * 5 * bullet->direction->z);
}

void System::reflectBullet(Obj3D *bullet) {
    if (bullet->reflected)
        return;

    glm::vec3 reflection = glm::cross(*bullet->direction, camera->calculateUp());
    bullet->direction = &reflection;
    bullet->reflected = true;
}

void System::processCollisions(int i) {
    for (int j = 0; j < objects.size(); ++j) {
        if (!objects[j]->isProjectile){
            if(objects[i]->testCollision(objects[j]))
            {
                if (objects[j]->canBeDestroyed){
                    objects[j]->destroyed = true;
                } else {
                    reflectBullet(objects[i]);
                }
            };
        }
    }
}

void System::Finish()
{
	coreShader.Delete();
	glfwTerminate();
}




