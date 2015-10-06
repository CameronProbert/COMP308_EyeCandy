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
  void renderEye(bool);
  void enableTextures();
  void enableShader(bool);
  void disableShader(bool);
  void setCorneaDiffuse();
  void setCorneaSpecular();
public:
  Scene(int);
  void renderScene(bool);
  void rotate();
  void lookAt(float, float);
  void setIrisColour(int);
  void setIrisColour();
};
