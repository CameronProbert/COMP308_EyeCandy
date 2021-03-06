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

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "geometry.hpp"
#include "sphereGeometry.hpp"

using namespace std;
using namespace comp308;


SphereGeometry::SphereGeometry(string filename) {
	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0) {
		createDisplayListPoly();
		createDisplayListWire();
	}
}


void SphereGeometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));


	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;

	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		//int i = 0;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {


				vector<vertex> verts;
				while (objLine.good()){
					vertex v;

					objLine >> v.p;		// Scan in position index

					// Scan in notmals and uv if present
					if(objLine.peek()=='/'){

						objLine.ignore(1);	// Ignore the '/' character

						// Scan in the uv if present
						if(objLine.peek()!='/'){
							objLine >> v.t;		// Scan in uv (texture coord) index
						}

						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.n;		// Scan in normal index
					}

					verts.push_back(v);
				}

				// IFF we have 3 verticies, construct a triangle
				if(verts.size() == 3){
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);

				}
			}
		}
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;


	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();
}

//-------------------------------------------------------------
// Uses the same index for a point as the index for the points normal
//-------------------------------------------------------------
void SphereGeometry::createNormals() {

	// Initialize normals
	for (unsigned int p = 0; p < m_points.size() -1; p++){
		vec3 init(0,0,0);
		m_normals.push_back(init);
	}

	vec3 norm, a, b, c;

	// For each face.
	for(unsigned int i=0; i<m_triangles.size(); i++){

		a = m_points[m_triangles[i].v[0].p];
		b = m_points[m_triangles[i].v[1].p];
		c = m_points[m_triangles[i].v[2].p];


		// Calculate face normal
		norm = normalize(cross((b - a),(c - a)));


		// For each vertex
		for(int j=0; j<3; j++){

			// Add the current face normal to the current normal for the vertex
			m_normals[m_triangles[i].v[j].p] += norm;

			// Make sure the vertex has a reference to its normal
			m_triangles[i].v[j].n = m_triangles[i].v[j].p;
		}
	}

	// Normalize
	for(unsigned n=0; n<m_normals.size(); n++){
		m_normals[n]=normalize(m_normals[n]);
	}

	cout << m_normals.size()-1 << " normals created" << endl;

}



void SphereGeometry::createDisplayListPoly() {
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);


	glBegin(GL_TRIANGLES);
	// For each face
	for(unsigned int i=0; i<m_triangles.size(); i++){
		// For each vertex
		for(int j=0; j<3; j++){

			vec3 vert = m_points[m_triangles[i].v[j].p];
			vec2 uv = m_uvs[m_triangles[i].v[j].t];
			vec3 norm = m_normals[m_triangles[i].v[j].n];

			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(uv.x * 5, uv.y * 5);
			glVertex3f(vert.x, vert.y, vert.z);

		}
	}


	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEndList();
	cout << "Finished creating Poly Geometry" << endl;
}



void SphereGeometry::createDisplayListWire() {
	// Delete old list if there is one
	if (m_displayListWire) glDeleteLists(m_displayListWire, 1);


	// Create a new list
	cout << "Creating Wire Geometry" << endl;
	m_displayListWire = glGenLists(1);
	glNewList(m_displayListWire, GL_COMPILE);


	glBegin(GL_TRIANGLES);
	// For each face
	for(unsigned int i=0; i<m_triangles.size(); i++){
		// For each vertex
		for(int j=0; j<3; j++){

			vec3 vert = m_points[m_triangles[i].v[j].p];
			//vec2 uv = m_uvs[m_triangles[i].v[j].t];
			vec3 norm = m_normals[m_triangles[i].v[j].n];

			glNormal3f(norm.x, norm.y, norm.z);
			//glTexCoord2f(uv.x, uv.y);
			glVertex3f(vert.x, vert.y, vert.z);
		}
	}

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEndList();
	cout << "Finished creating Wire Geometry" << endl;
}


void SphereGeometry::renderGeometry() {
	if (m_wireFrameOn) {

		glShadeModel(GL_SMOOTH);
		//glutWireTeapot(5.0);
		glCallList(m_displayListWire);

	} else {

		glShadeModel(GL_SMOOTH);
		//glutSolidTeapot(5.0);
		glCallList(m_displayListPoly);

	}
}


void SphereGeometry::toggleWireFrame() {
	m_wireFrameOn = !m_wireFrameOn;
}


void SphereGeometry::setAmbient(float a, float b, float c){
	m_material.a[0] = a; m_material.a[1] = b; m_material.a[2] = c;
}

void SphereGeometry::setDiffuse(float a, float b, float c){
	m_material.d[0] = a; m_material.d[1] = b; m_material.d[2] = c;
}

void SphereGeometry::setSpecular(float a, float b, float c){
	m_material.s[0] = a; m_material.s[1] = b; m_material.s[2] = c;
}

void SphereGeometry::setShininess(float s){
	m_material.shininess = s;
}

material SphereGeometry::getMat(){
	return m_material;
}