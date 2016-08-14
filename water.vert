#version 330 core

//Define position, normal, and texture defined in the Container.
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

//Define uniform MVP: model, view, projection passed from the object.
uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float tiling = 6.0;

//Define any out variables for the fragment shader.
out vec4 clipSpace;
out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragTexCoords;

void main()
{
	clipSpace = MVP * vec4(vertex.x, 0.0, vertex.z, 1.0);
    gl_Position = clipSpace;

	FragPos = vec3(model * vec4(vertex.x, vertex.y, vertex.z, 1.0));
	FragNormal = vec3( mat4(transpose(inverse(model)))  * vec4(normal.x, normal.y, normal.z, 1.0f));
	FragTexCoords = vec2(texCoords.x/2.0 + 0.5, texCoords.y/2.0 + 0.5) * tiling;
}
