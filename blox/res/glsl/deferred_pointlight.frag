#version 400 core

//Falloff
const float cFalloff = 5.0f;

//ViewProjection matrices
uniform mat4 uViewProjection;
uniform mat4 uInverseViewProjection;

//Camera position in world space
uniform vec3 uCameraPosition;

//GBuffer textures
uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferNormal;
uniform sampler2D uGBufferDepth;

//Point light characteristics
struct PointLight
{
	vec3 position;
	vec3 color;	
	float radius;
};
uniform PointLight uPointLight;

//Input: Texture coordinates received from vertex shader
in vec2 fTexCoord;

//Output: Pixel color
out vec4 pColor;

float getIntensity( void )
{
	float depth = texture(uGBufferDepth, fTexCoord).x * 2.0f - 1.0f;
	vec3 viewPosition = vec3(fTexCoord * 2.0f - 1.0f, depth);
	vec4 worldPosition = uInverseViewProjection * vec4(viewPosition, 1.0f);
	worldPosition.xyz /= worldPosition.w;

	//Offset vector from light source
	vec3 offset = uPointLight.position - worldPosition.xyz;

	//Distance from light to camera
	float distance = length(offset);
	if(distance > uPointLight.radius) discard;

	//Surface normal
	vec3 normal = normalize(texture(uGBufferNormal, fTexCoord).xyz);

	float i = exp(-distance / uPointLight.radius * cFalloff);
	i *= dot(offset, normal);
	return max(i, 0.0f);
}

vec3 getColor( void )
{	
	//Return product of diffuse and light color
	return uPointLight.color * texture(uGBufferDiffuse, fTexCoord).rgb;
}

void main( void )
{
	pColor = vec4(getColor() * getIntensity(), 1.0f);
}