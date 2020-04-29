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

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CW );

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	coreShader = Shader( "resources/shaders/core/core.vert", "resources/shaders/core/core.frag" );
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WIDTH), static_cast<GLfloat>(this->HEIGHT), 0.0f, -1.0f, 1.0f);
    coreShader.Use();

	return EXIT_SUCCESS;
}

void System::Run()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;


    coreShader.Use();
	coreShader.LoadTexture( "resources/textures/woodTexture.jpg", "texture1", "woodTexture" );
    coreShader.LoadTexture( "resources/textures/bricks.jpg", "texture1", "bricksTexture" );


    Obj3D *cube = obj3DReader->readObjFromFile("/Users/i848340/unisinos/compgraf/resources/objects/cubo.obj");
    cube->scale = 0.5f;
    cube->rotate = 0.0f;
    cube->translate = new glm::vec3(5.0f, 5.0f, -15.0f);
    objects.push_back(cube);

    Obj3D *sphere = obj3DReader->readObjFromFile("/Users/i848340/unisinos/compgraf/resources/objects/sphere.obj");
    sphere->scale = 0.5f;
    sphere->rotate = 0.0f;
    sphere->translate = new glm::vec3(0.0f, 0.0f, -15.0f);
    objects.push_back(sphere);

    Obj3D *objtest = new Obj3D();
    objtest->mesh = cube->mesh;
    objtest->scale = 0.5f;
    objtest->rotate = 0.0f;
    objtest->translate = new glm::vec3(15.0f, 0.0f, -15.0f);
    objects.push_back(objtest);

    std::vector<float> verticesVector;
    std::vector<float> texturesVector;
    std::vector<float> normalsVector;

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

                std::cout << "\n Vertex ID = " << j;
                std::cout << "\n Vertex count = " << face->verticesCount();


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

#pragma endregion

		glClearColor( 0.2f, 0.3f, 0.8f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		coreShader.Use();

		coreShader.UseTexture( "bricksTexture" );

        for (int i = 0; i < objects.size(); ++i) {
            objects[i]->draw(camera, &coreShader);
        }

		glfwSwapBuffers( window );
	}
}

void System::Finish()
{
	coreShader.Delete();
	glfwTerminate();
}


