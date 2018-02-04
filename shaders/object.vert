#version 330 core

// Define position, normal, and texture defined in the Container.
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

// Define uniform MVP: model, view, projection passed from the object.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec4 clipPlane;

//Define any out variables for the fragment shader.
out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragTexCoords;
out float visibility;

void main()
{
    gl_Position =  projection * view * model * vec4(vertex.x, vertex.y, vertex.z, 1.0f);
	FragPos = vec3(model * vec4(vertex.x, vertex.y, vertex.z, 1.0f));
	FragNormal = vec3( mat4(transpose(inverse(model)))  * vec4(normal.x, normal.y, normal.z, 1.0f));  
	FragTexCoords = texCoords;

	vec4 worldPosition = model * vec4(vertex.x, vertex.y, vertex.z, 1.0f);
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
}
