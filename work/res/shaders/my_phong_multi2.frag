//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#version 120

// Constant across both shaders
uniform sampler2D texture0;

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;



void main()
{

	vec4 final = vec4 (0.0,0.0,0.0,0.0);

	for(int light = 1; light < 2; light++){



		// normalize vectors after interpolation
	   vec3 L = normalize(gl_LightSource[light].position.xyz - vPosition);
	   vec3 V = normalize(-vPosition);
	   vec3 N = normalize(-reflect(L,vNormal));

	   // get Blinn-Phong reflectance components

	   // Ambient
	   vec4 Iamb = gl_FrontLightProduct[light].ambient;

	   // Diffuse
	   float diffuseTerm = clamp(dot(N,L),0,1);
	   vec4 Idif = gl_FrontLightProduct[light].diffuse * diffuseTerm;

	   // Specular
	   float specularTerm = 0;

	   // Calculate specular reflection only if the surface is orientated to the light
	   if(dot(N,L)>0){
	   	// half vector
	   	vec3 H = normalize(L+V);
	   	specularTerm = pow(dot(N, H), gl_FrontMaterial.shininess);

	   }
	   vec4 Ispe = gl_FrontLightProduct[light].specular * specularTerm;

	   // diffuse color of the object from texture
	   //vec3 diffuseColor = texture(u_diffuseTexture, vTextureCoord0).rgb;

	   final += Iamb + Idif + Ispe;
	 
	}

	vec4 colorMap = texture2D(texture0, vTextureCoord0);

	// write Total Color:  
	gl_FragColor =  gl_FrontLightModelProduct.sceneColor + final;
	
}