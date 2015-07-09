#version 400 core

//Uniform: World-View-Projection matrix
uniform mat4 uWVP;
uniform mat4 uWorld;

//Input: Vertex position
in vec3 vPosition;

//Input: Vertex normal
in vec3 vNormal;

//Input: Vertex color
in vec3 vColor;

//Output: Clip space position
out vec3 fPosition;

//Output: World space normal
out vec3 fNormal;

//Output: Color
out vec3 fColor;

void main( void )
{
	gl_Position = uWVP * vec4(vPosition, 1.0f);
	fPosition = gl_Position.xyz;
	fNormal = (uWorld * vec4(vNormal, 0.0f)).xyz;
	fColor = vColor;
}