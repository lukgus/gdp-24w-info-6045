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

	const int NUM_BONES = 50;

	std::string name;
	Shader vertexShader;
	Shader fragmentShader;
	GLuint id;

	GLuint ProjectionMatrixUL;
	GLuint ViewMatrixUL;
	GLuint ModelMatrixUL;
	GLuint RotationMatrixUL;
	GLuint BoneMatricesUL[50];
};

