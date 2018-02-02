#ifndef SHADER_HPP
#define SHADER_HPP

class Shader
{
private:
	GLuint m_Id;

public:
	Shader(const char * vertex_file_path, const char * fragment_file_path);
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	
	void Use();
	GLuint GetId() { return m_Id; }													// Reference to the shaderProgram

	void Set(const GLchar *name, GLfloat value);									// Float
	void Set(const GLchar *name, GLint value);										// Integer
	void Set(const GLchar *name, GLfloat x, GLfloat y);								// Vec2f
	void Set(const GLchar *name, const glm::vec2 &value);							// Vec2f
	void Set(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);					// Vec3f
	void Set(const GLchar *name, const glm::vec3 &value);							// Vec3f
	void Set(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);		// Vec4f
	void Set(const GLchar *name, const glm::vec4 &value);							// Vec4f
	void Set(const GLchar *name, const glm::mat4 &matrix);							// Mat4
};

#endif
