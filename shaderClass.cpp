#include"shaderClass.h"

std::string get_file_contents(const char* filename) 
{
	std::ifstream in(filename, std::ios::binary);
	if (in) 
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertextFile, const char* fragmentFile) 
{

	std::string vertexCode = get_file_contents(vertextFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//VERTEX SHDER

	//creating the Vertext Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attaching vertex shader to vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//compiling vertex shader
	glCompileShader(vertexShader);



	//FRAGMENT SHADER

	//creating Fragment Shader
	GLuint framentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//attaching fragmeent shader to fragment shader object
	glShaderSource(framentShader, 1, &fragmentSource, NULL);
	//compiling fragment shader
	glCompileShader(framentShader);

	//creating shader program and getting the correct refrence
	ID = glCreateProgram();

	//attaching vertext and fragment shaders to the shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, framentShader);

	//linking all shaders into the shader program
	glLinkProgram(ID);


	//deleting the un-used shaders after they have been linked to the program
	glDeleteShader(vertexShader);
	glDeleteShader(framentShader);

}


void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glUseProgram(ID);
}

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}