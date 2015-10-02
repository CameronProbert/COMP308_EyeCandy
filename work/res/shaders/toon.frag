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
varying vec3 vNormal, vLightDir;


void main()
{
	float intensity;
	vec3 n;
	vec4 color;

	n = normalize(vNormal);
	intensity = max(dot(vLightDir,n),0.0); 
	if (intensity > 0.98)
	color = vec4(0.8,0.8,0.8,1.0);
	else if (intensity > 0.5)
	color = vec4(0.4,0.4,0.8,1.0);
	else if (intensity > 0.25)
	color = vec4(0.2,0.2,0.4,1.0);
	else
	color = vec4(0.1,0.1,0.1,1.0);
	gl_FragColor = color;
}