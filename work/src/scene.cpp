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
GLuint g_irisTexture = 0;

// Reference to the shader
GLuint shader = 0;

float rotY = 0.0;
float stop = 0.0;

bool g_rotate = false;


Scene::Scene(int s) {
	shader = s;
  
  initTexture("../work/res/textures/irisBW256.jpg", &g_irisTexture);
  
	g_eyeball = new Geometry("../work/res/assets/eyeFull.obj"); // bunny
  g_eyeball->setAmbient(0.25, 0.20725, 0.20725);
	g_eyeball->setDiffuse(1, 0.829, 0.829);
	g_eyeball->setSpecular(0.296648, 0.296648, 0.296648);
	g_eyeball->setShininess(0.088);
}

void Scene::initTexture(std::string fileName, GLuint *texName) {
  image tex(fileName);

  glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
  glGenTextures(1, texName); // Generate texture ID
  glBindTexture(GL_TEXTURE_2D, *texName); // Bind it as a 2D texture
       
  // Setup sampling strategies
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Finally, actually fill the data into our texture
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());
}


void Scene::enableTextures(){
  // Enable Drawing texures
  glEnable(GL_TEXTURE_2D);
  // Use Texture as the color
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // GL_MODULATE can be replaced with GL_REPLACE
  // Set the location for binding the texture
  glActiveTexture(GL_TEXTURE0);
}

void Scene::renderScene(){
  //enableTextures();
	glPushMatrix(); 

    // rotate the eye to follow the mouse
		glRotatef(degrees(thetaX),1,0,0); // Rotate scene around x axis
		glRotatef(degrees(thetaY),0,1,0); // Rotate scene around y axis
		
		// Set the iris texture
		//glBindTexture(GL_TEXTURE_2D, g_irisTexture);
		
		// Render the eye
		glEnable(GL_POLYGON_STIPPLE);
		renderEye();


	glPopMatrix();
}


void Scene::lookAt(float x, float y){
  thetaX = y;
  thetaY = x;
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
