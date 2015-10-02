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
	void initTexture(std::string, GLuint *);
	void renderEye();
public:
	Scene(int);
	void renderScene();
	void rotate();
};
