#include "stdafx.h"
#include "Shader.h"


Shader::Shader(const char * vertex_file_path, const char * fragment_file_path)
{
	GLuint loaded_id = LoadShaders(vertex_file_path, fragment_file_path);
	m_Id = loaded_id;
}

GLuint Shader::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Check to make sure the file exists and is in the right directory !\n", vertex_file_path);
		printf("The current working directory is:");
#ifdef _WIN32
		system("CD");
#else
		system("pwd");
#endif
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Shader::Use()
{
	glUseProgram(m_Id);
}

void Shader::Set(const char * name, float value) const
{
	glUniform1f(glGetUniformLocation(m_Id, name), value);
}

void Shader::Set(const char * name, int value) const
{
	glUniform1i(glGetUniformLocation(m_Id, name), value);
}

void Shader::Set(const char * name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_Id, name), x, y);
}

void Shader::Set(const char * name, const glm::vec2 & value) const
{
	glUniform2f(glGetUniformLocation(m_Id, name), value.x, value.y);
}

void Shader::Set(const char * name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_Id, name), x, y, z);
}

void Shader::Set(const char * name, const glm::vec3 & value) const
{
	glUniform3f(glGetUniformLocation(m_Id, name), value.x, value.y, value.z);
}

void Shader::Set(const char * name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_Id, name), x, y, z, w);
}

void Shader::Set(const char * name, const glm::vec4 & value) const
{
	glUniform4f(glGetUniformLocation(m_Id, name), value.x, value.y, value.z, value.w);
}

void Shader::Set(const char * name, const glm::mat4 & matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, &(matrix[0][0]));
}
