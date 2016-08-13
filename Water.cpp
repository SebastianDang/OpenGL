
#include <stdio.h>
#include "Water.h"

#define SIZE 20
#define VERTEX_COUNT 2
#define DRAW_SHADED 0
#define DRAW_WIREFRAME 1

Water::Water(int x_d, int z_d)
{
	//Setup the terrain.
	this->x = x_d * SIZE;
	this->z = z_d * SIZE;
	this->draw_mode = DRAW_SHADED;
	//Setup toWorld so that the terrain is at the center of the world.
	this->toWorld = glm::mat4(1.0f);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-SIZE/2, 0, -SIZE/2));
	this->toWorld = translate * this->toWorld;
	//Setup HeightMap
	this->setupGeometry();
	//Load the texture and setup VAO, VBO for the terrains.
	this->setupWater();
	//Initialize the reflectionBuffer and refractionBuffer.
	this->initializeReflection();
	this->initializeRefraction();
}

/* Deconstructor to safely delete when finished. */
Water::~Water()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

/* Setup the Bezier surfaces. */
void Water::setupGeometry()
{
	//Generate vertices, normals, and texCoords for a terrain map. vertex = (j, i).
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			//Setup the vertices.
			float vertex_x = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			float vertex_y = 0.0f;
			float vertex_z = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
			//Setup the normals.
			float normal_x = 0;
			float normal_y = 1.0f;
			float normal_z = 0;
			//Setup the texcoords.
			float texCoord_x = (float)j / ((float)VERTEX_COUNT - 1);
			float texCoord_y = (float)i / ((float)VERTEX_COUNT - 1);
			//Push back to vectors.
			vertices.push_back(glm::vec3(vertex_x, vertex_y, vertex_z));
			normals.push_back(glm::vec3(normal_x, normal_y, normal_z));
			texCoords.push_back(glm::vec2(texCoord_x, texCoord_y));
		}
	}
	//Setup the indices to draw based on indice points.
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz*VERTEX_COUNT) + gx;//0//1
			int topRight = topLeft + 1;//1//2
			int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;//128//129
			int bottomRight = bottomLeft + 1;//129//130
			//Push back to indices.
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
	//Add into container structs for rendering.
	for (int i = 0; i < VERTEX_COUNT * VERTEX_COUNT; i++)
	{
		//Throw everything into a container to hold all values.
		Container container;
		container.vertex = vertices[i];
		container.normal = normals[i];
		container.texCoord = texCoords[i];
		containers.push_back(container);
	}
}

/* Setup water for glsl. Mostly contains the geometric components of the water. */
void Water::setupWater()
{
	//Create buffers/arrays.
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	//Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO); //Bind vertex array object.

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind Container buffer.
	glBufferData(GL_ARRAY_BUFFER, this->containers.size() * sizeof(Container), &this->containers[0], GL_STATIC_DRAW); //Set vertex buffer to the Container.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind indices buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_STATIC_DRAW);

	//Vertex Positions.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Container), (GLvoid*)0);

	//Vertex Normals.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Container), (GLvoid*)offsetof(Container, normal));

	//Vertex Texture Coords.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Container), (GLvoid*)offsetof(Container, texCoord));

	//Unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind.
	glBindVertexArray(0); //Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO.
}

/* Initialize the reflection frame buffer. */
void Water::initializeReflection()
{
	//Set the width and height of the frame buffer.
	int width = Window::width;
	int height = Window::height;
	//Generate and bind the frame buffer object.
	glGenFramebuffers(1, &this->reflection_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->reflection_FBO);
	//Generate the frame buffer texture.
	glGenTextures(1, &this->reflection_texture);
	glBindTexture(GL_TEXTURE_2D, reflection_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Generate the depth buffer as well.
	glGenRenderbuffers(1, &this->reflection_DBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->reflection_DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->reflection_DBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//Configure the frame buffer by binding both the texture (COLOR), and the depth (DEPTH).
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->reflection_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->reflection_DBO);
	//Check for any errors from the frame buffer.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Frame buffer not complete.\n");
	//Unbind the frame buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Initialize the refraction frame buffer. */
void Water::initializeRefraction()
{
	//Set the width and height of the frame buffer.
	int width = Window::width;
	int height = Window::height;
	//Generate and bind the frame buffer object.
	glGenFramebuffers(1, &this->refraction_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->refraction_FBO);
	//Generate the frame buffer texture.
	glGenTextures(1, &this->refraction_texture);
	glBindTexture(GL_TEXTURE_2D, refraction_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Generate the depth buffer as well.
	glGenRenderbuffers(1, &this->refraction_DBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->refraction_DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->refraction_DBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//Configure the frame buffer by binding both the texture (COLOR), and the depth (DEPTH).
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->refraction_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->refraction_DBO);
	//Check for any errors from the frame buffer.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Frame buffer not complete.\n");
	//Unbind the frame buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Toggle the draw mode to draw the mesh as lines (wireframe) or as triangle faces. */
void Water::toggleDrawMode()
{
	if (draw_mode == 0)
	{
		draw_mode = DRAW_WIREFRAME;
	}
	else
	{
		draw_mode = DRAW_SHADED;
	}
}

/* Draw the water mesh. */
void Water::draw(GLuint shaderProgram)
{
	//Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices. Send to shader.
	glm::mat4 MVP = Window::P * Window::V * toWorld;
	glm::mat4 model = this->toWorld;
	glm::mat4 view = Window::V;
	glm::mat4 projection = Window::P;
	//Set MVP(Total calculated, easier to multiply in the shader) for the shader.
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	//Set individual components for shader calculations (Model, View, Projection).
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
	//Update view position and time.
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), Window::camera_pos.x, Window::camera_pos.y, Window::camera_pos.z);
	//Set draw_mode to view wireframe version or filled version.
	if (draw_mode == DRAW_SHADED)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (draw_mode == DRAW_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
    //Bind for drawing.
    glBindVertexArray(VAO);
	//Bind the reflection texture.
	glActiveTexture(GL_TEXTURE0);//Enable the texture.
	glBindTexture(GL_TEXTURE_2D, this->reflection_texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), 0);
	//Bind the refraction texture.
	glActiveTexture(GL_TEXTURE1);//Enable the texture.
	glBindTexture(GL_TEXTURE_2D, this->refraction_texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), 1);
	//Draw the water.
    glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
	//Set it back to fill.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
