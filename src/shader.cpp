
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.h"

void Shader::attachShader(const char* shaderRelativePath, uint32 shaderType)
{
	std::filesystem::path vertexPath(shaderRelativePath);
	std::string FullPath(std::filesystem::canonical(vertexPath));
	_shaderID.push_back(glCreateShader(shaderType));

	// Read the Vertex Shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(FullPath, std::ios::in);

	if (ShaderStream.is_open()) 
	{
		std::stringstream sstr;
		sstr << ShaderStream.rdbuf();
		ShaderCode = sstr.str();
		ShaderStream.close();
	}
	else 
		ft_assert("Impossible to open vertexShader");

	int32 Result = GL_FALSE;
	int32 InfoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling shader : " << FullPath << std::endl;
	char const* VertexSourcePointer = ShaderCode.c_str();
	glShaderSource(_shaderID.back(), 1, &VertexSourcePointer, NULL);
	glCompileShader(_shaderID.back());

	// Check Vertex Shader
	glGetShaderiv(_shaderID.back(), GL_COMPILE_STATUS, &Result);
	glGetShaderiv(_shaderID.back(), GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) 
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(_shaderID.back(), InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::string ErrorMessage(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end());
		
		std::cout << ErrorMessage << std::endl;
	}

	glAttachShader(_programId, _shaderID.back());
}

void Shader::use(void)
{
	glUseProgram(_programId);
}

void Shader::deleteShader()
{
	for (uint32 shaderid : _shaderID){
		glDetachShader(_programId, shaderid);
	}
	for (uint32 shaderid : _shaderID){
		glDeleteShader(shaderid);
	}
}

void Shader::setMat4(const std::string &name, glm::mat4 mat4) const
{
    glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()),1, false, &mat4[0].x);
}
void Shader::setVec4(const std::string &name, glm::vec4 vec4) const
{
    glUniform4fv(glGetUniformLocation(_programId, name.c_str()), 1, &vec4.x);
}

void Shader::setVec3(const std::string &name, glm::vec3 vec3) const
{
    glUniform3fv(glGetUniformLocation(_programId, name.c_str()), 1, &vec3.x);
}

void Shader::setUint(const std::string &name, unsigned int index) const
{
    glUniform1ui(glGetUniformLocation(_programId, name.c_str()), index);
}

void Shader::setFloat(const std::string &name, float index) const
{
    glUniform1f(glGetUniformLocation(_programId, name.c_str()), index);
}
