#include <shader.h>

#include <fstream>

std::string LoadShaderFromFile(std::string filename)
{
	std::ifstream shaderFile(filename.c_str());

	if (!shaderFile.good())
		return std::string();

	std::string stringBuffer(std::istreambuf_iterator<char>(shaderFile),
		(std::istreambuf_iterator<char>()));
	return stringBuffer;
}


bool CompileShader(Shader* shader)
{
	glCompileShader(shader->id);

	GLint result = 0xDEADBEEF;
	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &result);

	if (!result)
		return false;

	return true;
}

ShaderProgram::ShaderProgram(const char* vertexFilepath, const char* fragmentFilepath) {
	Shader vertShader, fragShader;

	id = glCreateProgram();
	vertShader.id = glCreateShader(GL_VERTEX_SHADER);
	fragShader.id = glCreateShader(GL_FRAGMENT_SHADER);
	vertShader.filename = vertexFilepath;
	fragShader.filename = fragmentFilepath;

	vertShader.source = LoadShaderFromFile(vertShader.filename);
	fragShader.source = LoadShaderFromFile(fragShader.filename);

	if (vertShader.source.empty() || fragShader.source.empty())
		return;

	const GLchar* tmp;
	tmp = static_cast<const GLchar*>(vertShader.source.c_str());
	glShaderSource(vertShader.id, 1, (const GLchar**)&tmp, NULL);

	tmp = static_cast<const GLchar*>(fragShader.source.c_str());
	glShaderSource(fragShader.id, 1, (const GLchar**)&tmp, NULL);

	if (!CompileShader(&vertShader) || !CompileShader(&fragShader))
		return;

	glAttachShader(id, vertShader.id);
	glAttachShader(id, fragShader.id);
	glLinkProgram(id);

	// Add our new shader to our map
	vertexShader = vertShader;
	fragmentShader = fragShader;

	glUseProgram(id);

	ModelMatrixUL = glGetUniformLocation(id, "ModelMatrix");
	ViewMatrixUL = glGetUniformLocation(id, "ViewMatrix");
	ProjectionMatrixUL = glGetUniformLocation(id, "ProjectionMatrix");

	BoneMatricesUL[0] = glGetUniformLocation(id, "BoneMatrices[0]");
	BoneMatricesUL[1] = glGetUniformLocation(id, "BoneMatrices[1]");
	BoneMatricesUL[2] = glGetUniformLocation(id, "BoneMatrices[2]");
	BoneMatricesUL[3] = glGetUniformLocation(id, "BoneMatrices[3]");
	BoneMatricesUL[4] = glGetUniformLocation(id, "BoneMatrices[4]");
	// You can use a for loop with a string stream to generate the string of the
	// BoneMatrices[#]. 
}

ShaderProgram::~ShaderProgram() {

}