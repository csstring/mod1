
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GL/glew.h"
#include "shader.h"

void Shader::initialize(const char* vertexRelativePath,const char* fragmentRelativePath) 
{
	std::filesystem::path vertexPath(vertexRelativePath);
  std::filesystem::path fragmentPath(fragmentRelativePath);

	_vertexFullPath.assign(std::filesystem::canonical(vertexPath));
	_fragmentFullPath.assign(std::filesystem::canonical(fragmentPath));
	// Create the shaders
	uint32 VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	uint32 FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(_vertexFullPath, std::ios::in);

	if (VertexShaderStream.is_open()) 
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else 
		ft_assert("Impossible to open vertexShader");

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(_fragmentFullPath, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	int32 Result = GL_FALSE;
	int32 InfoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling shader : " << _vertexFullPath << std::endl;
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) 
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::string ErrorMessage(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}

	// Compile Fragment Shader
	std::cout << "Compiling shader : " << _fragmentFullPath << std::endl;
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::string ErrorMessage(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}

	// Link the program
	std::cout << "Linking program" << std::endl;
	_programId = glCreateProgram();
	glAttachShader(_programId, VertexShaderID);
	glAttachShader(_programId, FragmentShaderID);
	glLinkProgram(_programId);

	// Check the program
	glGetProgramiv(_programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(_programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::string ErrorMessage(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}

	glDetachShader(_programId, VertexShaderID);
	glDetachShader(_programId, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void Shader::use(void)
{
	glUseProgram(_programId);
}