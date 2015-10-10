#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> 
#include <cmath>

#include "comp308.hpp"
#include "geometry.hpp"
#include "imageLoader.hpp"
#include "shaderLoader.hpp"





class Scene {

private:
  float thetaX;
  float thetaY;
  
  void initTexture(std::string, GLuint *);
  void initTexture(std::vector<unsigned char>, GLuint *, int, int);
  void renderEye(bool);
  void enableTextures();
  void enableShader(bool);
  void disableShader(bool);
  void setCorneaDiffuse();
  void setCorneaSpecular();
  float calculatePupilDilation();
  void initialiseIrises();
  GLuint generateTexture(image realTex, image approxTex, float proportion);
public:
  Scene(int);
  void setLightDirections(std::vector<comp308::vec4>);
  void renderScene(bool);
  void rotate();
  void lookAt(float, float);
  void setIrisColour(int);
  void setIrisColour();
};
