#include "stdafx.h"
#include "Skybox.h"

#define SIZE 500.0f

Skybox::Skybox()
{
	ResetToWorld();

	m_Faces.push_back("assets/skybox/right.ppm");//Right.
	m_Faces.push_back("assets/skybox/left.ppm");//Left.
	m_Faces.push_back("assets/skybox/top.ppm");//Top.
	m_Faces.push_back("assets/skybox/bottom.ppm");//Bottom.
	m_Faces.push_back("assets/skybox/back.ppm");//Back.
	m_Faces.push_back("assets/skybox/front.ppm");//Front.

	GLfloat vertices_array[] =
	{
		// Front vertices
		-SIZE, -SIZE,  SIZE, // bottom left
		SIZE, -SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE, // top left

		// Back vertices
		-SIZE, -SIZE, -SIZE, // bottom left
		SIZE, -SIZE, -SIZE,
		SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE // top left
	};

	GLuint indices_array[] =
	{  // Note that we start from 0!
	   // Front face
		0, 1, 2,
		2, 3, 0,
		// Top face
		1, 5, 6,
		6, 2, 1,
		// Back face
		7, 6, 5,
		5, 4, 7,
		// Bottom face
		4, 0, 3,
		3, 7, 4,
		// Left face
		4, 5, 1,
		1, 0, 4,
		// Right face
		3, 2, 6,
		6, 7, 3
	};

	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	// Data.
	for (int i = 0; i <= 21; i += 3)
	{
		vertices.push_back(glm::vec3(vertices_array[i], vertices_array[i + 1], vertices_array[i + 2]));
		normals.push_back(glm::vec3(0.0f));
		texCoords.push_back(glm::vec2(0.0f));
	}

	for (int i = 35; i >= 0; i--)
		indices.push_back(indices_array[i]);

	// Load the data from the cube we created.
	int success = LoadData(vertices, normals, texCoords, indices, 24);
	if (success == 0)
	{
		// Create buffers/arrays.
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(m_VAO); //Bind vertex array object.

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO); //Bind Container buffer.
		glBufferData(GL_ARRAY_BUFFER, m_Data.size() * sizeof(S_Container), &m_Data[0], GL_STATIC_DRAW); // Set vertex buffer to the Container.

		// Vertex Positions.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(S_Container), (GLvoid*)offsetof(S_Container, m_Vertex));

		// Vertex Normals.
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(S_Container), (GLvoid*)offsetof(S_Container, m_Normal));

		// Vertex Texture Coords.
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(S_Container), (GLvoid*)offsetof(S_Container, m_TexCoord));

		// If there are indices, we can set them here, otherwise we don't use them.
		if ((int)m_Indices.size() > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); //Bind indices buffer.
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), &m_Indices[0], GL_STATIC_DRAW);
		}

		m_CubemapTexture = LoadCubemap(m_Faces);

		// Unbind.
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind.
		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO.
	}
}

Skybox::~Skybox()
{
}

/* Load the cube map and return a texture ID. */
GLuint Skybox::LoadCubemap(std::vector<const GLchar*> faces)
{
	//Hold the textureID (This will be the textureID to return).
	GLuint textureID;

	//Define variables to hold height map's width, height, pixel information.
	int width, height;
	unsigned char * image;

	//Create ID for texture.
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);//Set this texture to be the active texture (0).
	//Set this texture to be the one we are working with.
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	//Generate the texture.
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = Object::LoadPPM(faces[i], width, height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	//Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//X
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//Y
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//Z
	//Unbind the texture cube map.
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//Return the textureID, we need to keep track of this texture variable.
	return textureID;
}


void Skybox::Render(Shader * pShaderProgram)
{
	//Draw skybox.
	glDepthMask(GL_FALSE);// Remember to turn depth writing off.

	// Skybox cube.
	glBindVertexArray(m_VAO); // Bind the vertex.
	glActiveTexture(GL_TEXTURE0);// Enable the texture.
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);// Bind the cubemapTexture.
	glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);// Unbind vertex.
	glDepthMask(GL_TRUE);// Turn depth writing back on.

	// Culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

