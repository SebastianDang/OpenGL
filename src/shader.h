#pragma once

class Shader
{
private:
	GLuint m_Id;
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

public:
	Shader(const char * vertex_file_path, const char * fragment_file_path);
	
	void Use();
	GLuint GetId() { return m_Id; }													// Shader Id

	/// <summary>
	/// Set the shader property by name. This corresponds to the uniform variable in the shader.
	/// </summary>
	void Set(const char *name, float value) const;									// Float
	void Set(const char *name, int value) const;									// Integer
	void Set(const char *name, float x, float y) const;								// Vec2f
	void Set(const char *name, const glm::vec2 &value) const;						// Vec2f
	void Set(const char *name, float x, float y, float z) const;					// Vec3f
	void Set(const char *name, const glm::vec3 &value) const;						// Vec3f
	void Set(const char *name, float x, float y, float z, float w) const;			// Vec4f
	void Set(const char *name, const glm::vec4 &value) const;						// Vec4f
	void Set(const char *name, const glm::mat4 &matrix) const;						// Mat4
};
