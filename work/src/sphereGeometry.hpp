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
#include "geometry.hpp"

class SphereGeometry {
private:

	// Feilds for storing raw obj information
	std::string m_filename;
	std::vector<comp308::vec3> m_points;	// Point list
	std::vector<comp308::vec2> m_uvs;		// Texture Coordinate list
	std::vector<comp308::vec3> m_normals;	// Normal list
	std::vector<triangle> m_triangles;		// Triangle/Face list

	bool m_wireFrameOn = false;

	// IDs for the display list to render
	GLuint m_displayListPoly = 0; // DisplayList for Polygon
	GLuint m_displayListWire = 0; // DisplayList for Wireframe

	void readOBJ(std::string);

	void createNormals();

	void createDisplayListPoly();
	void createDisplayListWire();

public:
	material m_material;


	SphereGeometry(std::string);
	// ~GeometryManager();

	void renderGeometry();
	void toggleWireFrame();
	void setAmbient(float a, float b, float c);
	void setDiffuse(float a, float b, float c);
	void setSpecular(float a, float b, float c);
	void setShininess(float);
	material getMat();

};