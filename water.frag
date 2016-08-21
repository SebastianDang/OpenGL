#version 330 core

in vec4 clipSpace;
in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoords;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform float waveFactor;

const float waveStrength = 0.05;
const float amplitude = 0.3;
const float frequency = 10;
const float PI = 3.14159;

//Define out variable for the fragment shader: color.
out vec4 color;

void main()
{
	vec2 coords = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	vec2 reflectCoords = vec2(coords.x, -coords.y);
	vec2 refractCoords = vec2(coords.x, coords.y);

	//Distortion.
	vec2 distortion1 = (texture(dudvTexture, vec2(FragTexCoords.x + waveFactor, FragTexCoords.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(dudvTexture, vec2(-FragTexCoords.x + waveFactor, FragTexCoords.y + waveFactor)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;
	
	//Add to reflection and check bounds.
	reflectCoords += totalDistortion;
	reflectCoords.x = clamp(reflectCoords.x, 0.001, 0.999);	
	reflectCoords.y = clamp(reflectCoords.y, -0.999, -0.001);	
	
	//Add to refraction and check bounds.
	refractCoords += totalDistortion;
	refractCoords = clamp(refractCoords, 0.001, 0.999);

	//Result.
	vec4 reflectColor = texture(reflectionTexture, reflectCoords);
	vec4 refractColor = texture(refractionTexture, refractCoords);
	
    vec4 output_color = mix(reflectColor, refractColor, 0.7);
	output_color = mix(output_color, vec4(0.0, 0.3, 0.5, 1.0), 0.1);
	color = output_color;
}

    