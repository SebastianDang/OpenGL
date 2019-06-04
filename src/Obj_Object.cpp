#include "stdafx.h"
#include "Obj_Object.h"

Obj_Object::Obj_Object(const char* filepath)
	: Object()
{
	// Load the data from the filepath specified
	int success = LoadDataFromFile(filepath);
	if (success != -1) LoadDataIntoBuffers();
}

Obj_Object::~Obj_Object()
{
	// The buffer objects are cleared in the base class. Delete anything else created here.
}

int Obj_Object::LoadDataFromFile(const char* filepath)
{
	// Break up the filepath, so we can store it for later.
	std::string fullPath = std::string(filepath);
	int slash = (int)fullPath.find_last_of("\\/");
	if (slash != std::string::npos)
	{
		m_Path = fullPath.substr(0, slash);
		m_File = fullPath.substr(slash + 1);
	}

	// Initialize min, max, scale values for each coordinate.
	float min_X = INFINITY, min_Y = INFINITY, min_Z = INFINITY; // Minimum set to infinity so first value is always inputted.
	float max_X = -INFINITY, max_Y = -INFINITY, max_Z = -INFINITY; // Maximum set to -infinity so first value is always inputted.

	// Same for scale, used to find "max" scale or the longest axis. 
	// This ensures the ranges of vertices are [-1, 1].
	float avg_X = 0.0f, avg_Y = 0.0f, avg_Z = 0.0f, scale_v = -INFINITY;

	// Open the file for reading called objFile.
	std::FILE* objFile = fopen(filepath, "r");

	// If the file is unable to open return -1: fail.
	if (!objFile)
	{
		printf("Unable to read file: %s\n", filepath);
		return -1;
	}

	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	// Read the file until the end. "# are commments to be ignored".
	while (1) 
	{
		char buf[BUFSIZ];
		int check = fscanf(objFile, "%s", buf);
		if (check == EOF) break;

		// Read in lines that start with "v". Add into vertices.
		if (strcmp(buf, "v") == 0)
		{
			glm::vec3 vertex;
			if (fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z) >= 3)
			{
				vertices.push_back(vertex);

				// Calculate min, max for x, y, z.
				if (vertex.x < min_X) { min_X = vertex.x; }
				if (vertex.y < min_Y) { min_Y = vertex.y; }
				if (vertex.z < min_Z) { min_Z = vertex.z; }
				if (vertex.x > max_X) { max_X = vertex.x; }
				if (vertex.y > max_Y) { max_Y = vertex.y; }
				if (vertex.z > max_Z) { max_Z = vertex.z; }
			}
			continue;
		}

		// Read in lines that start with "vn". Add into normals.
		if (strcmp(buf, "vn") == 0) 
		{
			glm::vec3 normal;
			if (fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z) >= 3)
			{
				normals.push_back(normal);
			}
			continue;
		}

		// Read in lines that start with "vt". Add into textures.
		if (strcmp(buf, "vt") == 0) 
		{
			glm::vec2 texCoord;
			if (fscanf(objFile, "%f %f\n", &texCoord.x, &texCoord.y) >= 2)
			{
				texCoords.push_back(texCoord);
			}
			continue;
		}

		// Read in lines that start with "f". Add into indices.
		if (strcmp(buf, "f") == 0) {//Note: Read only left or right side since they are identical on both sides.
			unsigned int faces_v[3], faces_vn[3]; //faces_vt[3];
			//if (fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &faces_v[0], &faces_vt[0], &faces_vn[0], &faces_v[1], &faces_vt[1], &faces_vn[1], &faces_v[2], &faces_vt[2], &faces_vn[2]) >= 9)
			if (fscanf(objFile, "%d//%d %d//%d %d//%d\n", &faces_v[0], &faces_vn[0], &faces_v[1], &faces_vn[1], &faces_v[2], &faces_vn[2]) >= 6)
			{
				indices.push_back(faces_v[0] - 1);
				indices.push_back(faces_v[1] - 1);
				indices.push_back(faces_v[2] - 1);
			}
			continue;
		}
	}
	fclose(objFile);

	// Calculate average x, y, z.
	avg_X = (min_X + max_X) / 2;
	avg_Y = (min_Y + max_Y) / 2;
	avg_Z = (min_X + max_Z) / 2;

	// Calculate scale value.
	float scale_x = (max_X - min_X);
	float scale_y = (max_Y - min_Y);
	float scale_z = (max_Z - min_Z);
	if (scale_x > scale_v) { scale_v = scale_x; }
	if (scale_y > scale_v) { scale_v = scale_y; }
	if (scale_z > scale_v) { scale_v = scale_z; }

	// Subtract the average to center all objects and multiply by (1/scale) to bring them down to size.
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x = vertices[i].x - avg_X;
		vertices[i].y = vertices[i].y - avg_Y;
		vertices[i].z = vertices[i].z - avg_Z;
		vertices[i] *= (1.0f / (scale_v));
	}

	// Load the data from the obj we loaded.
	return LoadData(vertices, normals, texCoords, indices, (int)vertices.size());
}

void Obj_Object::Render(Shader * pShaderProgram)
{
	// If the buffers aren't loaded or the shader program isn't initialized, we don't render.
	if (!IsInit() || !pShaderProgram) return;

	// Update any variables that are with this object.
	pShaderProgram->Set("model", m_ToWorld);

	// Bind for rendering.
	glBindVertexArray(m_VAO);

	// Choose drawing method.
	int DataCount = (int)m_Data.size();
	int IndicesCount = (int)m_Indices.size();

	// Draw in the order data is defined. This is faster if data is chosen carefully.
	if (DataCount > 0 && IndicesCount <= 0)
		glDrawArrays(GL_TRIANGLES, 0, DataCount);

	// Draw in the order indices are defined.
	else if (DataCount > 0 && IndicesCount > 0)
		glDrawElements(GL_TRIANGLES, (GLsizei)IndicesCount, GL_UNSIGNED_INT, 0);

	// Unbind.
	glBindVertexArray(0);
}
