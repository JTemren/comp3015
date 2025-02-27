#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<STB/stb_image.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"


//vertcies for triangle
GLfloat vertices[] =
{	//			COORDINATES							/		COLOURS					//

	//			TRIFORCE
	//-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f,			// Lower left corner
	// 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f,			// Lower right corner
	// 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f,			// Upper corner
	//-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f,			// Inner left
	// 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f,			// Inner right
	// 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f			// Inner down


	//			SQUARE
	-0.5f, -0.5f, 0.0f,								1.0f, 0.0f, 0.0f,				0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,								0.0f, 1.0f, 0.0f,				0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,								0.0f, 0.0f, 1.0f,				1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,								1.0f, 1.0f, 1.0f,				1.0f, 0.0f  // Lower right corner

};

GLuint indices[] =
{
	//			TRIFORCE
	//0, 3, 5,	// Lower left triangle
	//3, 2, 4,	// Lower right triangle
	//5, 4, 1		// Upper triangle

	//			SQUARE
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

int main()
{
	// initalizing GLFW
	glfwInit();

	//giving GLFW context for what version we are using (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	// telling GLFW to us the core function profile 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creating a simple window
	GLFWwindow* window = glfwCreateWindow(800, 800, "COMP3015", NULL, NULL);

	//error checking the creation of thee GLFW window
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//giving GLFW context for the window
	glfwMakeContextCurrent(window);

	//load GLAD so it can use OpenGL
	gladLoadGL();

	//specify the viewpoint and size of the window
	glViewport(0, 0, 800, 800);


	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID the scale uniform
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture

	int widthImg, hightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("pop_cat.png", &widthImg, &hightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//	float flatColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColour);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, hightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0uni = glGetUniformLocation(shaderProgram.ID, "Tex0");
	shaderProgram.Activate();
	glUniform1i(tex0uni, 0);


	//Main while loop
	while (!glfwWindowShouldClose(window)) {

		//specify the colour of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//clear and assign a new colour to it 
		glClear(GL_COLOR_BUFFER_BIT);

		//assigning the correct shader program to use
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);

		//binding the VAO so OpenGL knows to use it
		VAO1.Bind();
		//draw the triangle using GL_TRIANGLES primitive
		//							 6 FOR SQARE, 9 FOR TRIFORCE
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//swap the back and front buffer
		glfwSwapBuffers(window);

		//handle GLFW events
		glfwPollEvents();
	}


	//delete all objects we have created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();

	//delete window before endinng the program
	glfwDestroyWindow(window);
	//terminate GLFW before the program ends
	glfwTerminate();


	std::cout << "Hello World!" << std::endl;
	return 0;
}