#pragma once

#include <GLEW/glew.h>
#include <string>

struct Shader {
	std::string filename;
	std::string source;
	bool bIsCompiled;
	GLuint id;
};

class ShaderProgram {
public:
	ShaderProgram(const char* vertexFilepath, const char* fragmentFilepath);
	~ShaderProgram();

	std::string name;
	Shader vertexShader;
	Shader fragmentShader;
	GLuint id;

	GLuint ProjectionMatrixUL;
	GLuint ViewMatrixUL;
	GLuint ModelMatrixUL;
	GLuint RotationMatrixUL;
};

