#include "StableFluidsShader.h"

void StableFluidsShader::initialize() 
{
	int32 Result = GL_FALSE;
	int32 InfoLogLength;

	_programId = glCreateProgram();
	attachShader("./stableFluidsScene/shaderSource/stableFluidsVertexShader.glsl", GL_VERTEX_SHADER);
	attachShader("./stableFluidsScene/shaderSource/stableFluidsFragmentShader.glsl", GL_FRAGMENT_SHADER);
	glLinkProgram(_programId);

	glGetProgramiv(_programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(_programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::string ErrorMessage(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		std::cout << ErrorMessage << std::endl;
	}
	deleteShader();
}