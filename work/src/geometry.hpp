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

#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "comp308.hpp"


struct vertex {
	int p = 0; // index for point in m_points
	int t = 0; // index for uv in m_uvs
	int n = 0; // index for normal in m_normals
};

struct triangle {
	vertex v[3]; //requires 3 verticies
};

struct material {
	float a[3]; //ambient
	float d[3]; //diffuse
	float s[3]; //specular
	float shininess = 0;
};

class Geometry {
private:

	// Feilds for storing raw obj information
	std::string m_filename;
	std::vector<comp308::vec3> m_points;	// Point list
	std::vector<comp308::vec2> m_uvs;		// Texture Coordinate list
	std::vector<comp308::vec3> m_normals;	// Normal list
	std::vector<comp308::vec3> m_norMain;	// Normal list
	std::vector<comp308::vec3> m_norIris;	// Normal list
	std::vector<comp308::vec3> m_norPupil;	// Normal list
	std::vector<comp308::vec3> m_norOther;	// Normal list
	std::vector<triangle> m_triangles;		// Triangle/Face list
	std::vector<triangle> m_triMain;		// Triangle/Face list
	std::vector<triangle> m_triIris;		// Triangle/Face list
	std::vector<triangle> m_triLens;		// Triangle/Face list
	std::vector<triangle> m_triCornea;		// Triangle/Face list
	
	std::string part = "Main"; 

	int current = 0;
	int  positions[8];
	
	bool m_wireFrameOn = false;

	// IDs for the display list to render
	GLuint m_displayListPoly = 0; // DisplayList for Polygon
	GLuint m_displayListWire = 0; // DisplayList for Wireframe

	void readOBJ(std::string);
	void copyFaces(std::vector<triangle> *, int, int);
	
	void renderSingleGeometry(std::vector<triangle> * ); 

	void createNormals();//std::vector<triangle>*, std::vector<comp308::vec3>*);

	void createDisplayListPoly();
	void createDisplayListWire();

public:
	material m_material;


	Geometry(std::string);
	// ~GeometryManager();

	void renderGeometry();
	
	void renderMain();
	void renderIris();
	void renderLens();
	void renderCornea();
	
	
	void setAmbient(float a, float b, float c);
	void setDiffuse(float a, float b, float c);
	void setSpecular(float a, float b, float c);
	void setShininess(float);
	material getMat();

};