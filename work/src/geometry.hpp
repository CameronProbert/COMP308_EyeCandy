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
#include <limits>

#include "comp308.hpp"


struct vertex {
	int p = 0; // index for point in m_points
	int t = 0; // index for uv in m_uvs
	int n = 0; // index for normal in m_normals
};

struct triangle {
	vertex v[3]; //requires 3 verticies
	// add a face normal here
	comp308::vec3 faceNormal;
};

struct material {
	float a[4]; //ambient
	float d[4]; //diffuse
	float s[4]; //specular
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
	std::vector<triangle> m_triFinal;		// Triangle/Face list


	int startPoint = std::numeric_limits<int>::max(); // arbitrary large value
	int finalPoint = 0;
	int current = 0;
	
	bool m_wireFrameOn = false;
	std::string currentDraw;

	// IDs for the display list to render
	GLuint m_displayListPoly = 0; // DisplayList for Polygon
	GLuint m_displayListWire = 0; // DisplayList for Wireframe

	void readOBJ(std::string);
	void copyFaces(std::vector<triangle> *, int, int);

	void createNormals();
	
	void createNormalsMean();
	void createNormalsMedian();

	void createDisplayListPoly();
	void createDisplayListWire();
	

public:
	material m_material;


	Geometry(std::string, std::string);
	// ~GeometryManager();

	void renderGeometry();
	
	void setAmbient(float a, float b, float c, float d);
	void setDiffuse(float a, float b, float c, float d);
	void setSpecular(float a, float b, float c, float d);
	void setShininess(float);
	material getMat();

};
