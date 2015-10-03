#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm> 

#include "comp308.hpp"
#include "scene.hpp"
#include "geometry.hpp"
#include "imageLoader.hpp"
#include "shaderLoader.hpp"

using namespace std;
using namespace comp308;

// Geometry for the scene

Geometry *g_eyeball = nullptr;

// Textures
GLuint g_tableTexture = 0;
GLuint g_boxTexture = 0;

// Reference to the shader
GLuint shader = 0;


Scene::Scene(int s) {
	shader = s;

	g_eyeball = new Geometry("../work/res/assets/eyeFull.obj"); // bunny
		g_eyeball->setAmbient(0.25, 0.20725, 0.20725);
		g_eyeball->setDiffuse(1, 0.829, 0.829);
		g_eyeball->setSpecular(0.296648, 0.296648, 0.296648);
		g_eyeball->setShininess(0.088);
}

float rotY = 0.0;
float stop = 0.0;

bool g_rotate = false;


void Scene::renderScene(){

	glPushMatrix(); 

		glRotatef(rotY,0,1,0); // Rotate scene around y axis

		if(g_rotate){
			rotY+=1.0;
			if(rotY == 360.0){
				rotY = 0.0;
			}
			if(rotY == stop){
				g_rotate = false;
			}
		}
		
		renderEye();


	glPopMatrix();
}



void Scene::renderEye(){
	material m = g_eyeball->m_material;

	glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);


	glPushMatrix(); 
		glScalef(0.1,0.1,0.1);
		g_eyeball->renderGeometry();
	glPopMatrix();
}


void Scene::rotate(){
	stop = rotY;
	g_rotate = !g_rotate;
}