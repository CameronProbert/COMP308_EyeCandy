#version 120

uniform float fresnelBias;
uniform float fresnelScale;
uniform float fresnelPower;

// Values to pass to the fragment shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;

varying float refFactor;



void main() {
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
	vTextureCoord0 = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// Empirical Approximation
	refFactor = max(0,min(1,(fresnelBias + fresnelScale * pow(1.0 + dot(normalize(vPosition - (-vPosition)), vNormal), fresnelPower))));

}