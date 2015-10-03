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

Geometry *g_eyeMain = nullptr;
Geometry *g_eyeIris = nullptr;
Geometry *g_eyeCornea = nullptr;
Geometry *g_eyeLens = nullptr;

// Textures
GLuint g_irisTexture = 0;

// Reference to the shader
GLuint shader = 0;


Scene::Scene(int s) {
	shader = s;
  
  initTexture("../work/res/textures/irisBW256.jpg", &g_irisTexture);
  
	g_eyeMain = new Geometry("../work/res/assets/eyeFull.obj", "Main"); 
	g_eyeIris = new Geometry("../work/res/assets/eyeFull.obj", "Iris"); 
	g_eyeLens = new Geometry("../work/res/assets/eyeFull.obj", "Lens"); 
	g_eyeCornea = new Geometry("../work/res/assets/eyeFull.obj", "Cornea"); 
	
	g_eyeMain->setAmbient(0.25, 0.20725, 0.20725, 1.f);
	g_eyeMain->setDiffuse(1, 0.829, 0.829, 1.f);
	g_eyeMain->setSpecular(0.296648, 0.296648, 0.296648, 1.f);
	g_eyeMain->setShininess(0.088);
	
	g_eyeIris->setAmbient(0.15, 0.05, 0.05, 1.f);
	g_eyeIris->setDiffuse(0.85, 0.2, 0.2, 1.f);
	g_eyeIris->setSpecular(0.296648, 0.1, 0.1, 1.f);
	g_eyeIris->setShininess(0.088);
	
	g_eyeLens->setAmbient(0.f, 0.f, 0.f, 1.f);
	g_eyeLens->setDiffuse(0.f, 0.f, 0.f, 1.f);
	g_eyeLens->setSpecular(0.f, 0.f, 0.f, 1.f);
	g_eyeLens->setShininess(0.f);
	
	g_eyeCornea->setAmbient(0.25, 0.20725, 0.20725, 0.5f);
	g_eyeCornea->setDiffuse(1, 0.829, 0.829, 0.5f);
	g_eyeCornea->setSpecular(0.296648, 0.296648, 0.296648, 0.5f);
	g_eyeCornea->setShininess(0.088);
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
	glPushMatrix(); 

    // rotate the eye to follow the mouse
		glRotatef(degrees(thetaX),1,0,0); // Rotate scene around x axis
		glRotatef(degrees(thetaY),0,1,0); // Rotate scene around y axis
		
		// Render the eye
		renderEye();


	glPopMatrix();
}


void Scene::lookAt(float x, float y){
  thetaX = y;
  thetaY = x;
}

void setMaterial(material m){
  glMaterialfv(GL_FRONT, GL_AMBIENT, m.a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.s);
	glMaterialf(GL_FRONT, GL_SHININESS, m.shininess * 128.0);
}


void Scene::renderEye(){

	glPushMatrix(); 
		glScalef(0.1,0.1,0.1);
		
		setMaterial(g_eyeMain->m_material);
		g_eyeMain->renderGeometry();
		
		enableTextures();
		glBindTexture(GL_TEXTURE_2D, g_irisTexture);
		setMaterial(g_eyeIris->m_material);
		g_eyeIris->renderGeometry();
		glDisable(GL_TEXTURE_2D);
		
		setMaterial(g_eyeLens->m_material);
		g_eyeLens->renderGeometry();
		
		glEnable(GL_BLEND);
		setMaterial(g_eyeCornea->m_material);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(0);
		//shaderManager.UseStockShader(GLT_SHADER_IDENTITY, g_eyeCornea->m_material);
		g_eyeCornea->renderGeometry();
		glDepthMask(1);
		glDisable(GL_BLEND);
		
	glPopMatrix();
}
