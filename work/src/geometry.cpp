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

using namespace std;
using namespace comp308;


Geometry::Geometry(string filename, string t) {
	currentDraw = t;
	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0) {
		renderGeometry();
	}
}


void Geometry::readOBJ(string filename) {

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
				//m_normals.push_back(vn);

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
						int normal;
						objLine >> normal;		// Scan in normal index
						//v.n = normal;
					}
					//cout << "vertex positions read in" << endl;
					if(!(v.p ==0 && v.t==0 && v.n==0)){
					  verts.push_back(v);
					}
				}
				// IFF we have 3 verticies, construct a triangle
				if(verts.size() == 3){
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);
					current++;	
				}
			}
			else if (mode == currentDraw){
				startPoint = current;
				cout << currentDraw << "----------------------------" << endl;
			}
			else if((currentDraw == "Main" && mode == "Iris") || (currentDraw == "Iris" && mode == "Cornea") || (currentDraw == "Cornea" && mode == "Lens")){
			  finalPoint=current;
			}
		}
	}
	if(currentDraw == "Lens") finalPoint = m_triangles.size()-1;
	
	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;
	//m_normals.clear();

	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();
}


bool sortByX (vec3 i, vec3 j) {return (i.x < j.x);} 
bool sortByY (vec3 i, vec3 j) {return (i.y < j.y);} 
bool sortByZ (vec3 i, vec3 j) {return (i.z < j.z);} 

bool isUnique (vec3 p, vector<vec3> nei){
 for (int i =0; i<nei.size(); i++){
  if(p.x == nei[i].x && p.y == nei[i].y && p.z == nei[i].z){
   return false;    
  }
 }  
 return true;
}

void Geometry::createNormals() {
	// Initialize normals
	for (int p = 0; p < m_points.size() -1; p++){
		vec3 init(0,0,0);
		m_normals.push_back(init);
	}

	vec3 norm, a, b, c;

	// For each face.
	for(int i=0; i<m_triangles.size(); i++){

		a = m_points[m_triangles[i].v[0].p];
		b = m_points[m_triangles[i].v[1].p];
		c = m_points[m_triangles[i].v[2].p];

		// Calculate face normal
		if(length(cross((b - a),(c - a)))!=0) norm = normalize(cross((b - a),(c - a)));

		// For each vertex
		for(int j=0; j<3; j++){

			// Add the current face normal to the current normal for the vertex
			m_normals[m_triangles[i].v[j].p] += norm;

			// Make sure the vertex has a reference to its normal
			m_triangles[i].v[j].n = m_triangles[i].v[j].p;
		}
	}
	

	// Normalize
	for(int n=0; n<m_normals.size(); n++){
	    if(length(m_normals[n])!=0){
		m_normals[n]=normalize(m_normals[n]);
	    }
	}

	cout << m_normals.size() << " normals created" << endl;

}

void Geometry::createNormalsMean() {
	// Initialize normals
	for (int p = 0; p < m_points.size() -1; p++){
		vec3 init(0,0,0);
		m_normals.push_back(init);
	}
	
	// do the smoothing
     for(int roll = 0; roll<3; roll++){
	
	for (int i=0; i<(int)m_triangles.size(); i++){
	  // find all the neighbour vertices for each vertex
	  
	 
	  for( int vertex = 0; vertex < 3; vertex++){
	    vector<vec3> neighbours;
	    //neighbours.push_back(m_points[m_triangles[i].v[vertex].p]);
	  
	    // for all the triangles
	    for (int j=0; j<(int)m_triangles.size(); j++){
	      // add the vectors that are neighbours
	      bool flag0 = isUnique(m_points[m_triangles[j].v[0].p], neighbours);
	      bool flag1 = isUnique(m_points[m_triangles[j].v[1].p], neighbours);
	      bool flag2 = isUnique(m_points[m_triangles[j].v[2].p], neighbours);
	      
	      bool add = false;
	      if(m_triangles[j].v[0].p == m_triangles[i].v[vertex].p || m_triangles[j].v[1].p == m_triangles[i].v[vertex].p || m_triangles[j].v[2].p == m_triangles[i].v[vertex].p){
		add = true;
	      }
	      if(add){
		if(flag0){
		  neighbours.push_back(m_points[m_triangles[j].v[0].p]);
		}
		if(flag1){
		  neighbours.push_back(m_points[m_triangles[j].v[1].p]);
		}
		if(flag2){
		  neighbours.push_back(m_points[m_triangles[j].v[2].p]);
		}
		
	      }
	    }
	    //cout << "got here 2" << endl;
	  
	    vec3 sum;
	    // go through all the neighbours
	    for(int n =0; n<neighbours.size(); n++){
	      sum = sum + neighbours[n];
	    }
	    // that vertex is now equal to the sum divided by the number of neighbours
	    vec3 newVer = sum;
	    newVer.x = newVer.x/neighbours.size();
	    newVer.y = newVer.y/neighbours.size();
	    newVer.z = newVer.z/neighbours.size();
	    
	    //cout << newVer.x << " x " << newVer.y << " y " << newVer.z << " z " << endl;
	    m_points[m_triangles[i].v[vertex].p] = newVer;
	    //cout << "got here 3" << endl;
	  
	  }
	}
      }

	vec3 norm, a, b, c;

	// For each face.
	for(int i=0; i<m_triangles.size(); i++){

		a = m_points[m_triangles[i].v[0].p];
		b = m_points[m_triangles[i].v[1].p];
		c = m_points[m_triangles[i].v[2].p];

		// Calculate face normal
		if(length(cross((b - a),(c - a)))!=0) norm = normalize(cross((b - a),(c - a)));

		// For each vertex
		for(int j=0; j<3; j++){

			// Add the current face normal to the current normal for the vertex
			m_normals[m_triangles[i].v[j].p] += norm;

			// Make sure the vertex has a reference to its normal
			m_triangles[i].v[j].n = m_triangles[i].v[j].p;
		}
	}
	

	// Normalize
	for(int n=0; n<m_normals.size(); n++){
	    if(length(m_normals[n])!=0){
		m_normals[n]=normalize(m_normals[n]);
	    }
	}

	cout << m_normals.size() << " normals created" << endl;

}

//-------------------------------------------------------------
// Uses the same index for a point as the index for the points normal
//-------------------------------------------------------------
void Geometry::createNormalsMedian(){

	// Initialize normals
	for (unsigned int p = 0; p < m_points.size() -1; p++){
		vec3 init(0,0,0);
		m_normals.push_back(init);
	}	
	
	// do the smoothing
     for(int roll = 0; roll<1; roll++){
	
	for (int i=0; i<(int)m_triangles.size(); i++){
	  // find all the neighbour vertices for each vertex
	  
	 
	  for( int vertex = 0; vertex < 3; vertex++){
	    vector<vec3> neighbours;
	    //neighbours.push_back(m_points[m_triangles[i].v[vertex].p]);
	  
	    // for all the triangles
	    for (int j=0; j<(int)m_triangles.size(); j++){
	      // add the vectors that are neighbours
	      bool flag0 = isUnique(m_points[m_triangles[j].v[0].p], neighbours);
	      bool flag1 = isUnique(m_points[m_triangles[j].v[1].p], neighbours);
	      bool flag2 = isUnique(m_points[m_triangles[j].v[2].p], neighbours);
	      
	      bool add = false;
	      if(m_triangles[j].v[0].p == m_triangles[i].v[vertex].p || m_triangles[j].v[1].p == m_triangles[i].v[vertex].p || m_triangles[j].v[2].p == m_triangles[i].v[vertex].p){
		add = true;
	      }
	      if(add){
		if(flag0 && m_triangles[j].v[0].p != m_triangles[i].v[vertex].p ){
		  neighbours.push_back(m_points[m_triangles[j].v[0].p]);
		}
		if(flag1 && m_triangles[j].v[1].p != m_triangles[i].v[vertex].p){
		  neighbours.push_back(m_points[m_triangles[j].v[1].p]);
		}
		if(flag2 && m_triangles[j].v[2].p != m_triangles[i].v[vertex].p){
		  neighbours.push_back(m_points[m_triangles[j].v[2].p]);
		}
		
	      }
	    }
	    //cout << "got here 2" << endl;
	   vec3 newVer;
	   std::sort ( neighbours.begin(), neighbours.end(), sortByX );
	   if((neighbours.size()%2) !=0 ){
	      newVer.x = neighbours[neighbours.size()/2].x;
	   }
	   else {
	      newVer.x = (neighbours[neighbours.size()/2].x + neighbours[neighbours.size()/2-1].x)/2;
	   }
	   std::sort ( neighbours.begin(), neighbours.end(), sortByY );
	   if((neighbours.size()%2) !=0 ){
	      newVer.y = neighbours[neighbours.size()/2].y;
	   }
	   else {
	      newVer.y = (neighbours[neighbours.size()/2].y + neighbours[neighbours.size()/2-1].y)/2;
	   }
	   std::sort ( neighbours.begin(), neighbours.end(), sortByZ );
	   if((neighbours.size()%2) !=0 ){
	      newVer.z = neighbours[neighbours.size()/2].z;
	   }
	   else {
	      newVer.z = (neighbours[neighbours.size()/2].z + neighbours[neighbours.size()/2-1].z)/2;
	   }
	    
	    
	    //cout << newVer.x << " x " << newVer.y << " y " << newVer.z << " z " << endl;
	    m_points[m_triangles[i].v[vertex].p] = newVer;
	    //cout << "got here 3" << endl;
	  
	  }
	}
      }
      
      // do edge smoothing on the normals
      
     vec3 norm, a, b, c;

	// For each face.
	for(int i=0; i<m_triangles.size(); i++){

		a = m_points[m_triangles[i].v[0].p];
		b = m_points[m_triangles[i].v[1].p];
		c = m_points[m_triangles[i].v[2].p];

		// Calculate face normal
		if(length(cross((b - a),(c - a)))!=0) norm = normalize(cross((b - a),(c - a)));

		// For each vertex
		for(int j=0; j<3; j++){

			// Add the current face normal to the current normal for the vertex
			m_normals[m_triangles[i].v[j].p] += norm;

			// Make sure the vertex has a reference to its normal
			m_triangles[i].v[j].n = m_triangles[i].v[j].p;
		}
	}
	

	// Normalize
	for(int n=0; n<m_normals.size(); n++){
	    if(length(m_normals[n])!=0){
		m_normals[n]=normalize(m_normals[n]);
	    }
	}
      

      cout << m_normals.size()-1 << " normals created" << endl;
}



void Geometry::renderGeometry() {
	glShadeModel(GL_SMOOTH);
	//glutSolidTeapot(5.0);
	glCallList(m_displayListPoly);
	
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	//cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);
	

	glBegin(GL_TRIANGLES);
	// For each face
	for(unsigned int i=startPoint; i<finalPoint; i++){
		// For each vertex
		//vec3 norm = m_triangles[i].faceNormal;
		for(int j=0; j<3; j++){

			vec3 vert = m_points[m_triangles[i].v[j].p];
			vec2 uv = m_uvs[m_triangles[i].v[j].t];
			vec3 norm = m_normals[m_triangles[i].v[j].n];

			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(uv.x, uv.y);
			glVertex3f(vert.x, vert.y, vert.z);

		}
	}
	//cout << finalPoint << " the final point of " << currentDraw << endl;


	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEndList();
	//cout << "Finished creating Poly Geometry" << endl;
}



void Geometry::setAmbient(float a, float b, float c, float d){
	m_material.a[0] = a; 
	m_material.a[1] = b; 
	m_material.a[2] = c;
	m_material.a[3] = d;
}

void Geometry::setDiffuse(float a, float b, float c, float d){
	m_material.d[0] = a; 
	m_material.d[1] = b; 
	m_material.d[2] = c;
	m_material.d[3] = d;
}

void Geometry::setSpecular(float a, float b, float c, float d){
	m_material.s[0] = a; 
	m_material.s[1] = b; 
	m_material.s[2] = c;
	m_material.s[3] = d;
}

void Geometry::setShininess(float s){
	m_material.shininess = s;
}

material Geometry::getMat(){
	return m_material;
}
 
