#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<STB/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const unsigned int width = 800;
const unsigned int hight = 800;

//vertcies for triangle
GLfloat vertices[] =
{	//			COORDINATES							/		COLOURS					//

	//			Pyramid
	-0.5f, 0.0f,  0.5f,								0.83f, 0.70f, 0.44f,				0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,								0.83f, 0.70f, 0.44f,				5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,								0.83f, 0.70f, 0.44f,				0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,								0.83f, 0.70f, 0.44f,				5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,								0.92f, 0.86f, 0.76f,				2.5f, 5.0f

};

GLuint indices[] =
{

	//			Pyramid
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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
	GLFWwindow* window = glfwCreateWindow(width, hight, "COMP3015", NULL, NULL);

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
	glViewport(0, 0, width, hight);


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
	unsigned char* bytes = stbi_load("Textures/brick.png", &widthImg, &hightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, hightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0uni = glGetUniformLocation(shaderProgram.ID, "Tex0");
	shaderProgram.Activate();
	glUniform1i(tex0uni, 0);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);


	//Main while loop
	while (!glfwWindowShouldClose(window)) {

		//specify the colour of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//clear and assign a new colour to it 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//assigning the correct shader program to use
		shaderProgram.Activate();

		glfwSwapInterval(1);

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) {
			rotation += 0.5f;
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(width / hight), 0.1f, 100.0f);

		int modelloc = glGetUniformLocation(shaderProgram.ID,"model");
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

		int viewloc = glGetUniformLocation(shaderProgram.ID,"view");
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

		int projloc = glGetUniformLocation(shaderProgram.ID,"proj");
		glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(proj));


		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);

		//binding the VAO so OpenGL knows to use it
		VAO1.Bind();
		//draw the triangle using GL_TRIANGLES primitive
		//							 6 FOR SQARE, 9 FOR TRIFORCE
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
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