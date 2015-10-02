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

Geometry *g_table = nullptr;
Geometry *g_torus = nullptr;
Geometry *g_sphere = nullptr;
Geometry *g_bunny = nullptr;
Geometry *g_box = nullptr;
Geometry *g_teapot = nullptr;

// Textures
GLuint g_tableTexture = 0;
GLuint g_boxTexture = 0;

// Reference to the shader
GLuint shader = 0;


Scene::Scene(int s) {
	shader = s;

	g_table = new Geometry("../work/res/assets/table.obj"); // table
		initTexture("../work/res/textures/wood.jpg", &g_tableTexture);
		g_table->setAmbient(0.0, 0.0, 0.0);
		g_table->setDiffuse(0.4, 0.4, 0.4);
		g_table->setSpecular(0.8, 0.8, 0.8);
		g_table->setShininess(0.2);

	g_torus = new Geometry("../work/res/assets/torus.obj"); // torus
		g_torus->setAmbient(0.0, 0.0, 0.0);
		g_torus->setDiffuse(0.7, 0.0, 0.0);
		g_torus->setSpecular(0.7, 0.6, 0.6);
		g_torus->setShininess(0.25);


	g_sphere = new Geometry("../work/res/assets/sphere.obj"); // sphere
		g_sphere->setAmbient(0.2125, 0.1275, 0.0054);
		g_sphere->setDiffuse(0.814, 0.5284, 0.28144);
		g_sphere->setSpecular(0.493548, 0.371906, 0.266721);
		g_sphere->setShininess(0.2);

	g_bunny = new Geometry("../work/res/assets/bunny.obj"); // bunny
		g_bunny->setAmbient(0.25, 0.20725, 0.20725);
		g_bunny->setDiffuse(1, 0.829, 0.829);
		g_bunny->setSpecular(0.296648, 0.296648, 0.296648);
		g_bunny->setShininess(0.088);

	g_box = new Geometry("../work/res/assets/box.obj"); // box
		initTexture("../work/res/textures/brick.jpg", &g_boxTexture);
		g_box->setAmbient(0.4, 0.4, 0.4);
		g_box->setDiffuse(0.4, 0.4, 0.4);
		g_box->setSpecular(0.4, 0.4, 0.4);
		g_box->setShininess(0.1);

	g_teapot = new Geometry("../work/res/assets/teapot.obj"); // teapot
		g_teapot->setAmbient(0.135, 0.2225, 0.2575);
		g_teapot->setDiffuse(0.54, 0.69, 0.83);
		g_teapot->setSpecular(0.316228, 0.316228, 0.316228);
		g_teapot->setShininess(0.1);
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
		
		renderTable();
		renderTorus();
		renderSphere();
		renderBunny();
		renderBox();
		renderTeapot();


	glPopMatrix();
}

void Scene::renderTable(){

	glUniform1i(glGetUniformLocation(shader, "texture"), GL_TRUE);

	material m = g_table->m_material;

	glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);


	//// Enable Drawing texures
	glEnable(GL_TEXTURE_2D);
	// Use Texture as the color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Set the location for binding the texture
	glActiveTexture(GL_TEXTURE0);
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, g_tableTexture);



	glPushMatrix(); 
		//glTranslatef(5, 1, -4);
		g_table->renderGeometry();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(shader, "texture"), GL_FALSE);
}

void Scene::renderTorus(){
	material m = g_torus->m_material;

	glShadeModel(GL_SMOOTH);  
	glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);


	glPushMatrix(); 
		glTranslatef(5, 1, 5);
		g_torus->renderGeometry();
	glPopMatrix();
}

void Scene::renderSphere(){
	material m = g_sphere->m_material;

	glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);


	glPushMatrix(); 
		glTranslatef(-5, 2, 4);
		g_sphere->renderGeometry();
	glPopMatrix();
}

void Scene::renderBunny(){
	material m = g_bunny->m_material;

	glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);


	glPushMatrix(); 
		glTranslatef(0, 0.5, 0);
		g_bunny->renderGeometry();
	glPopMatrix();
}

void Scene::renderBox(){
	glUniform1i(glGetUniformLocation(shader, "texture"), GL_TRUE);

	material m = g_box->m_material;

	glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);

	// Enable Drawing texures
	glEnable(GL_TEXTURE_2D);
	// Use Texture as the color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Set the location for binding the texture
	glActiveTexture(GL_TEXTURE0);
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, g_boxTexture);


	glPushMatrix(); 
		glTranslatef(5, 2.5, -4);
		g_box->renderGeometry();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(shader, "texture"), GL_FALSE);
}

void Scene::renderTeapot(){
	material m = g_teapot->m_material;

	glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);

	glPushMatrix(); 
		glTranslatef(-5, 0, -4);
		g_teapot->renderGeometry();
	glPopMatrix();
}







// Method to load textures
void Scene::initTexture(string f, GLuint *i) {

	image tex(f);

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, i); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, *i); // Bind it as a 2D texture
	
	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());

	cout << tex.w << endl;
}

void Scene::rotate(){
	stop = rotY;
	g_rotate = !g_rotate;
}