#version 330 core

in vec4 clipSpace;
in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoords;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

//Define out variable for the fragment shader: color.
out vec4 color;

void main()
{
	vec2 coords = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	vec2 reflectCoords = vec2(coords.x, -coords.y);
	vec2 refractCoords = vec2(coords.x, coords.y);

	vec4 reflectColor = texture(reflectionTexture, reflectCoords);
	vec4 refractColor = texture(refractionTexture, refractCoords);
	
    color = mix(reflectColor, refractColor, 0.8);
}

    