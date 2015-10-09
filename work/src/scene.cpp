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
GLuint g_allTextures [20];
int g_currentTexture = 19;

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
  initialiseIrises();
  
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

int *** imageTo2D(image im){
  cout << "imageTo2D..." << endl;
  int width = im.w;
  int height = im.h;
  
  int x = 0;
  int y = 0;
  int col = 0;
  //cout << "im.data is: " << im.data[2]+0 << endl;
  // Unpack image vector to 2 dimensions
  
  cout << "Creating int array..." << endl;
  int *** unpackedTex = 0;
  unpackedTex = new int**[width];
  for (int w = 0; w < width; w++)
  {
    unpackedTex[w] = new int*[height];
    for (int h = 0; h < height; h++)
    {
      unpackedTex[w][h] = new int[3];
      for (int c = 0; c < 3; c++)
      {
       unpackedTex[w][h][c] = 0;
      }
    }
  }
  
  cout << "Loading pixels into array..." << endl;
  for (unsigned char pixel : im.data){
    unpackedTex[x][y][col] = pixel+0;
    //cout << "Pixel is: " << pixel+0 << endl;
    
    col++;
    if (col >=3){
      y++;
      col = 0;
      if (y >= height){
        x++;
        y = 0;
      }
    }
  }
  cout << "Returning unpacked texture..." << endl;
  return unpackedTex;
}

vector<unsigned char> vectorFrom2D(int *** array, int xLen, int yLen, int zLen){
  vector<unsigned char> vec;
  for (int x = 0; x < xLen; x++){
    for (int y = 0; y < yLen; y++){
      for (int z = 0; z < zLen; z++){
        vec.push_back(array[x][y][z]);
      }
    }
  }
  return vec;
}

/**
  * Generates a texture for the iris with the given proportion
  * approxTex is a black and white image used for finding the inside and outside edges of the iris
  */
GLuint Scene::generateTexture(image realTex, image approxTex, float proportion){
  GLuint num; // Variable for the texture to be bound to and returned
  cout << "Generating texture..." << endl;
  
  int width = approxTex.w;
  int height = approxTex.h;
  int centrePixelX = width/2;
  int centrePixelY = height/2;
  
  int x = 0;
  int y = 0;
  
  // Unpack image vectors to 2 dimensions + colour dimension
  // Stored as x, y, colour
  int *** unpackedApproxTex = imageTo2D(approxTex);
  int *** unpackedRealTex = imageTo2D(realTex);
  //int newTexture[width][height][3];
  int *** newTexture = 0;
  newTexture = new int**[width];
  for (int w = 0; w < width; w++)
  {
    newTexture[w] = new int*[height];
    for (int h = 0; h < height; h++)
    {
      newTexture[w][h] = new int[3];
      for (int c = 0; c < 3; c++)
      {
       newTexture[w][h][c] = 0;
      }
    }
  }
  
  // Go through every pixel
  // If pixel is outside the iris then add it directly to the next image
  // Otherwise find the angle from the centre the pixel is
    // Step in that angle until you find the outside of iris pixel,
    // Step in that angle until you find the inside of iris pixel,
    // Find the pixel that should be placed into the current pixel
  for (int i = 0; i < width; i++){
    for (int j = 0; j < height; j++){
      
      if (unpackedApproxTex[x][y][0] < 50){
        newTexture[x][y][0] = unpackedRealTex[x][y][0];
        newTexture[x][y][1] = unpackedRealTex[x][y][1];
        newTexture[x][y][2] = unpackedRealTex[x][y][2];
      }
      else {
        // Find the angle
        float angle = atan((centrePixelY-y)/((float)(centrePixelX-x))); // in Radians
        cout << "Angle in radians: " << angle << endl;
        angle = degrees(angle); // Degrees
        cout << "Angle in degrees: " << angle << endl;
        float prevColour = 0;
        
        int innerPixX = -1;
        int innerPixY = -1;
        int outerPixX = -1;
        int outerPixY = -1;
        
        // Eastern Quadrant
        if (angle > 45 && angle <= 135){
          // First pass, find the inner and outer points of the iris
          for (int xPixel = centrePixelX; xPixel < width; xPixel++){
            int yPixel = (centrePixelX-xPixel) * tan(angle);
            if (unpackedApproxTex[xPixel][yPixel][0] != prevColour){
              if (innerPixX == -1){
                innerPixX = xPixel;
                innerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              } else if (outerPixX == -1){
                outerPixX = xPixel;
                outerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              }
            }
          }
          // Second pass, find the pixel that goes into this index
          int pixelX = abs(x - innerPixX)/abs(outerPixX - innerPixX);
          int pixelY = abs(y - innerPixY)/abs(outerPixY - innerPixY);
          newTexture[x][y][0] = unpackedRealTex[pixelX][pixelY][0];
          newTexture[x][y][1] = unpackedRealTex[pixelX][pixelY][1];
          newTexture[x][y][2] = unpackedRealTex[pixelX][pixelY][2];
        }
        
        // Western Quadrant
        else if (angle > 225 && angle <= 305){
          // First pass, find the inner and outer points of the iris
          for (int xPixel = centrePixelX; xPixel >= 0; xPixel--){
            int yPixel = (centrePixelX-xPixel) * tan(angle);
            if (unpackedApproxTex[xPixel][yPixel][0] != prevColour){
              if (innerPixX == -1){
                innerPixX = xPixel;
                innerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              } else if (outerPixX == -1){
                outerPixX = xPixel;
                outerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              }
            }
          }
          // Second pass, find the pixel that goes into this index
          int pixelX = abs(x - innerPixX)/abs(outerPixX - innerPixX);
          int pixelY = abs(y - innerPixY)/abs(outerPixY - innerPixY);
          newTexture[x][y][0] = unpackedRealTex[pixelX][pixelY][0];
          newTexture[x][y][1] = unpackedRealTex[pixelX][pixelY][1];
          newTexture[x][y][2] = unpackedRealTex[pixelX][pixelY][2];
        }
        
        // Southern Quadrant
        else if (angle > 135 && angle <= 225){
          // First pass, find the inner and outer points of the iris
          for (int yPixel = centrePixelY; yPixel < height; yPixel++){
            int xPixel = (centrePixelY-yPixel) * tan(angle);
            if (unpackedApproxTex[xPixel][yPixel][0] != prevColour){
              if (innerPixX == -1){
                innerPixX = xPixel;
                innerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              } else if (outerPixX == -1){
                outerPixX = xPixel;
                outerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              }
            }
          }
          // Second pass, find the pixel that goes into this index
          int pixelX = abs(x - innerPixX)/abs(outerPixX - innerPixX);
          int pixelY = abs(y - innerPixY)/abs(outerPixY - innerPixY);
          newTexture[x][y][0] = unpackedRealTex[pixelX][pixelY][0];
          newTexture[x][y][1] = unpackedRealTex[pixelX][pixelY][1];
          newTexture[x][y][2] = unpackedRealTex[pixelX][pixelY][2];
        }
        
        // Northern Quadrant
        else if (angle > 305 || angle <= 45){
          // First pass, find the inner and outer points of the iris
          for (int yPixel = centrePixelY; yPixel >= 0; yPixel--){
            int xPixel = (centrePixelY-yPixel) * tan(angle);
            if (unpackedApproxTex[xPixel][yPixel][0] != prevColour){
              if (innerPixX == -1){
                innerPixX = xPixel;
                innerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              } else if (outerPixX == -1){
                outerPixX = xPixel;
                outerPixY = yPixel;
                prevColour = unpackedApproxTex[xPixel][yPixel][0];
              }
            }
          }
          // Second pass, find the pixel that goes into this index
          int pixelX = abs(x - innerPixX)/abs(outerPixX - innerPixX);
          int pixelY = abs(y - innerPixY)/abs(outerPixY - innerPixY);
          newTexture[x][y][0] = unpackedRealTex[pixelX][pixelY][0];
          newTexture[x][y][1] = unpackedRealTex[pixelX][pixelY][1];
          newTexture[x][y][2] = unpackedRealTex[pixelX][pixelY][2];
        }
      }
      
    }
  }
  
  vector<unsigned char> tex = vectorFrom2D(newTexture, width, height, 3);
  
  // Pack image vector to 1 dimension
  
  initTexture(tex, &num, width, height); // Bind the texture
  return num;
}

void Scene::initialiseIrises(){
  int count = 1;
  int maxCount = 20;
  image approxTexture("../work/res/textures/irisApprox256.jpg");
  image realTexture("../work/res/textures/irisBW256.jpg");
  //cout << "approxTexture[0] is: " << approxTexture.data[2000] << endl;
  //for( int i=0; i<approxTexture.data.size(); i++){
  //  std::cout << "approxTexture: " << approxTexture.data[i]+0 << endl;
  //}
  g_allTextures[0] = g_irisTexture;
  while (count < maxCount){
    g_allTextures[count] = generateTexture(realTexture, approxTexture, ((float)count)/((float)maxCount));
    cout << "Texture loaded into index: " << count << endl;
    count++;
  }
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


void Scene::initTexture(vector<unsigned char> tex, GLuint *texName, int w, int h) {
  glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
  glGenTextures(1, texName); // Generate texture ID
  glBindTexture(GL_TEXTURE_2D, *texName); // Bind it as a 2D texture
       
  // Setup sampling strategies
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Finally, actually fill the data into our texture
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, &tex[0]);
}


void Scene::enableTextures(){
  // Enable Drawing texures
  glEnable(GL_TEXTURE_2D);
  // Use Texture as the color
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // GL_MODULATE can be replaced with GL_REPLACE
  // Set the location for binding the texture
  glActiveTexture(GL_TEXTURE1);
  // Bind the texture
  glBindTexture(GL_TEXTURE_2D, g_allTextures[g_currentTexture]);

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
  //cout << "Dilation is: " << dilation << endl;
  
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
			// TODO set correct texture
			g_currentTexture = (int)(dilation*20);
			if (g_currentTexture > 19){
			  g_currentTexture = 19;
			}
      //cout << "Dilation is: " << g_currentTexture << endl;
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
