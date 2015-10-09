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

// External lights shining on the eye
// Made up of normalized direction (vec3) * strength (float)
vector<vec3> lightDirs;

enum {
  BROWN = 0,
  GREEN,
  BLUE,
  AQUA,
  CHESTNUT,
  HAZEL,
  GREY,
  VIOLET,
  COOL_GREEN,
  WHITE
};

std::string colourNames[] {
  "Brown",
  "Green",
  "Blue",
  "Aqua",
  "Chestnut",
  "Hazel",
  "Grey",
  "Violet",
  "Cool Green",
  "White"
};

float eyeColours[10][6] {
  // RED, GREEN, BLUE, ALPHA, AMBIENT, SPECULAR
  {83.0f, 60.0f, 44.0f, 1.0f, 0.15, 0.1},
  {63.0f, 69.0f, 41.0f, 1.0f, 0.15, 0.1},
  {78.0f, 108.0f, 146.0f, 1.0f, 0.15, 0.1},
  {108.0f, 117.0f, 126.0f, 1.0f, 0.15, 0.1},
  {101.0f, 80.0f, 75.0f, 1.0f, 0.15, 0.1},
  {121.0f, 84.0f, 66.0f, 1.0f, 0.15, 0.1},
  {112.0f, 112.0f, 112.0f, 1.0f, 0.15, 0.1},
  {124.0f, 126.0f, 183.0f, 1.0f, 0.15, 0.1},
  {57.0f, 81.0f, 45.0f, 1.0f, 0.15, 0.1},
  {170.0f, 170.0f, 170.0f, 1.0f, 0.15, 0.1}
};

int currentColour = HAZEL;


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
        
	setIrisColour(currentColour);
	g_eyeIris->setShininess(0.088);
	
	g_eyeLens->setAmbient(0.f, 0.f, 0.f, 1.f);
	g_eyeLens->setDiffuse(0.f, 0.f, 0.f, 1.f);
	g_eyeLens->setSpecular(0.f, 0.f, 0.f, 1.f);
	g_eyeLens->setShininess(0.f);
	
	g_eyeCornea->setAmbient(0.25, 0.20725, 0.20725, 0.1f);
	g_eyeCornea->setDiffuse(1, 0.829, 0.829, 0.1f);
	g_eyeCornea->setSpecular(1.f, 1.f, 1.f, 1.0f);
	g_eyeCornea->setShininess(0.188);
}

void Scene::setCorneaSpecular(){
  g_eyeCornea->setAmbient(0.0f, 0.0f, 0.0f, 0.0f);
  g_eyeCornea->setDiffuse(0.0f, 0.0f, 0.0f, 0.0f);
  g_eyeCornea->setSpecular(1.f, 1.f, 1.f, 0.6f);
  g_eyeCornea->setShininess(0.188);
}

void Scene::setCorneaDiffuse(){
  g_eyeCornea->setAmbient(0.25, 0.20725, 0.20725, 0.05f);
  g_eyeCornea->setDiffuse(1, 0.829, 0.829, 0.05f);
  g_eyeCornea->setSpecular(0.0f, 0.0f, 0.0f, 0.0f);
  g_eyeCornea->setShininess(0.188);
}

void Scene::setIrisColour(int index){
  float red = eyeColours[index][0]/255.f;
  float green = eyeColours[index][1]/255.f;
  float blue = eyeColours[index][2]/255.f;
  float transparency = eyeColours[index][3];
  float ambientMod = eyeColours[index][4];
  float specularMod = eyeColours[index][5];
  g_eyeIris->setAmbient(red*ambientMod, green*ambientMod, blue*ambientMod, transparency);
        g_eyeIris->setDiffuse(red, green, blue, transparency);
        g_eyeIris->setSpecular(red*specularMod, green*specularMod, blue*specularMod, transparency);
  cout << "Eye colour set to: " << colourNames[index] << endl;
}

void Scene::setLightDirections(vector<vec4> newLightDirs){
  lightDirs.clear();
  for (vec4 vec : newLightDirs){
    vec3 direction = {vec[0], vec[1], vec[2]};
    normalize(direction);
    direction *= vec[3];
    lightDirs.push_back(direction);
  }
}

void Scene::setIrisColour(){
  //srand(time[0]);
  //int col = randInt(BROWN, WHITE);
  int col = rand() % WHITE;
  setIrisColour(col);
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
  glActiveTexture(GL_TEXTURE1);
  // Bind the texture
  glBindTexture(GL_TEXTURE_2D, g_irisTexture);

  // Set our sampler (texture0) to use GL_TEXTURE0 as the source
  glUniform1i(glGetUniformLocation(shader, "texture0"), 1);
}

void Scene::renderScene(bool g_shader){
	glPushMatrix(); 

    // rotate the eye to follow the mouse
		glRotatef(degrees(thetaX),1,0,0); // Rotate scene around x axis
		glRotatef(degrees(thetaY),0,1,0); // Rotate scene around y axis
		
		// Render the eye
		renderEye(g_shader);


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

void Scene::enableShader(bool g_shader){
  //if (g_shader){
    // Set our sampler (texture0) to use GL_TEXTURE0 as the source
    glUniform1i(glGetUniformLocation(g_shader, "texture0"), 1);
    // Turns on using the texture in the shader
    glUniform1i(glGetUniformLocation(g_shader, "texture"), GL_TRUE);
    //glUniform1i(1, true);
  //}
}

void Scene::disableShader(bool g_shader){
  //if (g_shader){
    // Turns off using the texture in the shader
    glUniform1i(glGetUniformLocation(g_shader, "texture"), GL_FALSE);
    //glUniform1i(1, false);
  //}
}

float Scene::calculatePupilDilation(){
  vec3 eyeRotation = {0,0,1}; // Eye facing forward initially
  
  // Do x rotation
  mat3 xRot = {
    {1, 0, 0},
    {0, cos(thetaX), -sin(thetaX)},
    {0, sin(thetaX), cos(thetaX)}
  };
  eyeRotation = xRot * eyeRotation;
  
  // Do y rotation
  mat3 yRot = {
    {cos(thetaY), 0, sin(thetaY)},
    {0, 1, 0},
    {-sin(thetaY), 0, cos(thetaY)}
  };
  eyeRotation = yRot * eyeRotation;
  
  normalize(eyeRotation);
  float dotProd = 0;
  //cout << "x: " << eyeRotation.x << " || y: " << eyeRotation.y << " || z: " << eyeRotation.z << endl;
  for (vec3 vec : lightDirs){
    dotProd += dot(eyeRotation, vec);
  }
  //cout << "LightMagnitude: " << dotProd << endl;
  
  return abs(dotProd);
}


void Scene::renderEye(bool g_shader){

  float dilation = calculatePupilDilation();
  cout << "Dilation is: " << dilation << endl;
  
	glPushMatrix(); 
		glScalef(0.1,0.1,0.1);
		
		////// Main Eye //////
		glPushMatrix(); 
			setMaterial(g_eyeMain->m_material);
			g_eyeMain->renderGeometry();
		glPopMatrix();
		
		////// Iris //////
		glPushMatrix();
			setMaterial(g_eyeIris->m_material);
			enableTextures();
			glUniform1i(glGetUniformLocation(g_shader, "texture"), GL_TRUE);
			g_eyeIris->renderGeometry();
			glUniform1i(glGetUniformLocation(g_shader, "texture"), GL_FALSE);
			glDisable(GL_TEXTURE_2D);
        glPopMatrix();
		
		////// Lens //////
		glPushMatrix();
		  glScalef(1.3f, 1.3f, 1.3f);
		  glTranslatef(0, 0, dilation*6-10);
			setMaterial(g_eyeLens->m_material);
			g_eyeLens->renderGeometry();
    glPopMatrix();

		////// Cornea //////
		glPushMatrix();
			glEnable(GL_BLEND);
            setCorneaDiffuse();
			setMaterial(g_eyeCornea->m_material);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(0);
			//shaderManager.UseStockShader(GLT_SHADER_IDENTITY, g_eyeCornea->m_material);
			g_eyeCornea->renderGeometry();
			glDepthMask(1);
            setCorneaSpecular();
            setMaterial(g_eyeCornea->m_material);
            g_eyeCornea->renderGeometry();
			glDisable(GL_BLEND);
		glPopMatrix();
		
	glPopMatrix();
}
