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
#include <cstdlib>
#include <iostream>
#include <string>

#include "comp308.hpp"
#include "imageLoader.hpp"
#include "shaderLoader.hpp"
#include "scene.hpp"

#define PI 3.14159265

using namespace std;
using namespace comp308;


// Global variables
// 
GLuint g_winWidth  = 640;
GLuint g_winHeight = 480;
GLuint g_mainWindow = 0;


// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_mouseDown = false;
vec2 g_mousePos;
float g_yRotation = 0;
float g_xRotation = 0;
float g_zoomFactor = 1.0;


// Scene information
//
GLuint g_texture = 0;
GLuint g_shader = 0;
GLuint transparent_shader = 0;
bool g_useShader = true;

// Scene loader and drawer
//
Scene *g_scene = nullptr;


// Sets up where and what the light is
// Called once on start up
// 

float lx = -4.0;
float ly = 3.0;
float lz = 0.0;

void initLight() {


	// Enable all lighting
	//glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);
}


int num_maps = 4;

//// Lake ////
 char *faces[24] = {
  "../work/res/textures/right.jpg",
  "../work/res/textures/left.jpg",  
  "../work/res/textures/top.jpg", 
  "../work/res/textures/bottom.jpg", 
  "../work/res/textures/back.jpg", 
  "../work/res/textures/front.jpg", 

//// Saint Lazrus Church ////
  "../work/res/textures/saintLazarusChurch/posx.jpg",
  "../work/res/textures/saintLazarusChurch/negx.jpg",
  "../work/res/textures/saintLazarusChurch/posy.jpg",
  "../work/res/textures/saintLazarusChurch/negy.jpg",
  "../work/res/textures/saintLazarusChurch/posz.jpg",
  "../work/res/textures/saintLazarusChurch/negz.jpg",

//// Lancellotti Chapel ////
  "../work/res/textures/LancellottiChapel/posx.jpg",
  "../work/res/textures/LancellottiChapel/negx.jpg",
  "../work/res/textures/LancellottiChapel/posy.jpg",
  "../work/res/textures/LancellottiChapel/negy.jpg",
  "../work/res/textures/LancellottiChapel/posz.jpg",
  "../work/res/textures/LancellottiChapel/negz.jpg",

//// Niagra Falls ////
  "../work/res/textures/NiagraFalls/posx.jpg",
  "../work/res/textures/NiagraFalls/negx.jpg",
  "../work/res/textures/NiagraFalls/posy.jpg",
  "../work/res/textures/NiagraFalls/negy.jpg",
  "../work/res/textures/NiagraFalls/posz.jpg",
  "../work/res/textures/NiagraFalls/negz.jpg"
};

int map = 6;

void map_images(){

	for(int i = map; i < (map + 6); i++){
		image face(faces[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (i - (map)), 0, GL_RGB, face.w, face.h, 0, GL_RGB, GL_UNSIGNED_BYTE, face.dataPointer());
	}	
}


void initCubeMap(){

	map_images();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT); // else GL_CLAMP
  	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT); // else GL_CLAMP

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

	glEnable(GL_TEXTURE_CUBE_MAP); 
}



void initShader() {
	g_shader = makeShaderProgram("../work/res/shaders/phong.vert", "../work/res/shaders/phong.frag");
}


// Sets up where the camera is in the scene
// Called every frame
// 
void setUpCamera() {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, float(g_winWidth) / float(g_winHeight), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -50 * g_zoomFactor);
	glRotatef(g_xRotation, 1, 0, 0);
	glRotatef(g_yRotation, 0, 1, 0);
}

// Spotlight variables
float spot_cut = 40.0;
float spot_x = 0.0;
float spot_y = 8.0;
float spot_z = 0.0;
float spot_dirX = 0.0;
float spot_dirY = -1.0;
float spot_dirZ = 0.0;

// Ambient Color
float amb_color = 0.0; // 0.4

// Directional Light Variables
float dir_diffuse = 0.0; // 0.8
float dir_specular = 0.0; // 0.8
float dir_ambient = 0.0; // 0.4

// Bias, Scale, Power
float def_fresnel[3] = {0.02, 0.06, 1.0};
float max_fresnel[3] = {0.0, 1.0, 1.0};

// Fresnel Variables
float fresnelBias = 0.0; // 0.02
float fresnelScale = 0.0; // 0.06
float fresnelPower = 0.0; // 1.0

// Eye Rotation
float rotation = 0.0;
bool followMouse = false;
float thetaX;
float thetaY;

bool startUpSeq = false;
int my_count = 0;

void startUp(){
	my_count += 1;
	fresnelBias += 0.00004;
	fresnelScale += 0.00012;
	fresnelPower += 0.002;

	dir_diffuse += 0.0016;
	dir_specular += 0.0016;
	dir_ambient += 0.0008;

	amb_color += 0.0008;

	if(my_count >= 499){
		startUpSeq = false;
	}

}

void setLightDirections(vector<vec4> lightDirs){
  g_scene->setLightDirections(lightDirs);
}

float calculateStrength(float diffuse[], float specular[]){
  float diffuseStr = diffuse[0] + diffuse[1] + diffuse[2];
  diffuseStr /= 3.f;
  float specularStr = specular[0] + specular[1] + specular[2];
  specularStr /= 3.f;
  return 0.7f*diffuseStr + 0.3*specularStr;
}


// Draw function
//
void draw() {

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	

	setUpCamera();

	///////////////////// Lighting ////////////////////////

	float direction[]	  = {0.0f, 0.0f, -1.0f, 0.0f};
	float diffintensity[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float ambient[]       = {0.2f, 0.2f, 0.2f, 1.0f};


	// glLightfv(GL_LIGHT0, GL_POSITION, direction);
	// glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	// glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);	

	// Weak ambient light
	float ambientColor[] = {amb_color, amb_color, amb_color, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	// Weak directional light
	float directionalDiffuse[] = {dir_diffuse, dir_diffuse, dir_diffuse, 1.0f};
	float directionalSpecular[] = {dir_specular, dir_specular, dir_specular, 1.0f};
	float directionalAmbient[] = {dir_ambient, dir_ambient, dir_ambient, 1.0f};
	float directionalPos[] = {0.8f, 0.8f, 0.8f, 0.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, directionalAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, directionalSpecular);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, directionalDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, directionalPos);
	
	// Weak point light
	//float pointDiffuse[] = {0.4f, 0.4f, 0.4f, 1.0f};
	//float pointSpecular[] = {0.6f, 0.6f, 0.6f, 1.0f};
	//float pointPos[] = {-4.0, 3.0, 0.0, 1.0f};
	//GLfloat pointExponent = 8.0; 

	//glLightfv(GL_LIGHT2, GL_DIFFUSE, pointDiffuse);
	//glLightfv(GL_LIGHT2, GL_SPECULAR, pointSpecular);
	//glLightfv(GL_LIGHT2, GL_POSITION, pointPos);
	//glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, &pointExponent);


	// Strong spotlight
	//float spotDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//float spotSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};
	//float spotPos[] = {spot_x, spot_y, spot_z, 1.0f};
	//float spotDir[] = {spot_dirX, spot_dirY, spot_dirZ};
	//GLfloat spotCutOff = spot_cut; 
	//GLfloat spotExponent = 1.0; 

	//glLightfv(GL_LIGHT3, GL_DIFFUSE, spotDiffuse);
	//glLightfv(GL_LIGHT3, GL_SPECULAR, spotSpecular);
	//glLightfv(GL_LIGHT3, GL_POSITION, spotPos);
	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	//glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, &spotCutOff);
	//glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDir);
	//glLightfv(GL_LIGHT3, GL_SPOT_EXPONENT, &spotExponent);


	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	

	if(!followMouse){
  	g_scene->lookAt(rotation, 0.0);
  	rotation += 0.02;
  	if(rotation > 360){
  		rotation = 0.0;
  	}
  	g_scene->lookAt(rotation, 0.0);
  }
  else{
  	g_scene->lookAt(thetaX, thetaY);
  }

  vector<vec4> lightDirs;
  vec4 directionalLight = {
        directionalPos[0], 
        directionalPos[1], 
        directionalPos[2],
        calculateStrength(directionalDiffuse, directionalSpecular)
  };
  lightDirs.push_back(directionalLight);
	setLightDirections(lightDirs);




	// Use the shader we made
	glUseProgram(g_shader);

	glUniform1f(glGetUniformLocation(g_shader, "fresnelBias"), fresnelBias);
	glUniform1f(glGetUniformLocation(g_shader, "fresnelScale"), fresnelScale);
	glUniform1f(glGetUniformLocation(g_shader, "fresnelPower"), fresnelPower);

	g_scene->renderScene(g_shader);

	// Unbind our shader
	glUseProgram(0);

	




	// Disable flags for cleanup (optional)
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);


	// Move the buffer we just drew to the front
	glutSwapBuffers();

	// Queue the next frame to be drawn straight away
	glutPostRedisplay();

	if(startUpSeq){
		startUp();
	}
}


// Reshape function
// 
void reshape(int w, int h) {
    if (h == 0) h = 1;

	g_winWidth = w;
	g_winHeight = h;
    
    // Sets the openGL rendering window to match the window size
    glViewport(0, 0, g_winWidth, g_winHeight);  
}

bool dir_trigger = true;



// Keyboard callback
// Called once per button state change
//
void keyboardCallback(unsigned char key, int x, int y) {
	cout << "Keyboard Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...

	switch(key){
		case 'q':
			fresnelBias = max(0.0, fresnelBias - 0.02);
			break;
		case 'w':
			fresnelBias = min(1.0, fresnelBias + 0.02);
			break;
		case 'a':
			fresnelScale = max(0.0, fresnelScale - 0.02);
			break;
		case 's':
			fresnelScale = min(1.0, fresnelScale + 0.02);
			break;
		case 'z':
			fresnelPower = max(0.0, fresnelPower - 0.02);
			break;
		case 'x':
			fresnelPower = min(1.0, fresnelPower + 0.02);
			break;
		case 't':
      		g_scene->setIrisColour();
      		break;
      	case 'g':
      		startUpSeq = true;
      		break;
      	case 32:
      		rotation = 0.0;
      		followMouse = !followMouse;
      		break;
      	case 'm':
      		map += 6;
      		if(map >= (num_maps * 6)){
      			map = 0;
      		}
      		map_images();
      		glutPostRedisplay();
      		break;
	}
	cout << "fresnelBias: " << fresnelBias << endl;
	cout << "fresnelScale: " << fresnelScale << endl;
	cout << "fresnelPower: " << fresnelPower << endl;

	glUniform1f(glGetUniformLocation(g_shader, "fresnelBias"), fresnelBias);
	glUniform1f(glGetUniformLocation(g_shader, "fresnelScale"), fresnelScale);
	glUniform1f(glGetUniformLocation(g_shader, "fresnelPower"), fresnelPower);
}


// Special Keyboard callback
// Called once per button state change
//
void specialCallback(int key, int x, int y) {
	cout << "Special Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
}

bool eye = true;
// Mouse Button Callback function
// Called once per button state change
// 
void mouseCallback(int button, int state, int x, int y) {
	//cout << "Mouse Callback :: button=" << button << ", state=" << state << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	switch(button){
		case 2: // right mouse button
			if (state==0){
				eye = !eye;

				if(eye){
					map = 6;
					fresnelBias = def_fresnel[0];
					fresnelScale = def_fresnel[1];
					fresnelPower = def_fresnel[2];
				}
				else{
					map = 0;
					fresnelBias = max_fresnel[0];
					fresnelScale = max_fresnel[1];
					fresnelPower = max_fresnel[2];
				}
				map_images();
				g_scene->toggleEye();

			break;
			}
				
	}
}

// Mouse Motion Callback function
// Called once per frame if the mouse has moved and
// at least one mouse button has an active state
// 
void mouseMotionCallback(int x, int y) {
  //cout << "Mouse Motion Callback :: x,y=(" << x << "," << y << ")" << endl;
  // YOUR CODE GOES HERE
  // ...
	thetaX = atan((x-g_winWidth/2.f)/(g_winWidth-g_winWidth/5.f));
	thetaY = atan((y-g_winHeight/2.f)/(g_winHeight-g_winHeight/5.f));

	if(followMouse){
		g_scene->lookAt(thetaX, thetaY);
	}
}



//Main program
// 
int main(int argc, char **argv) {
  srand((unsigned)time(NULL)); 
	if(argc != 1){
		cout << "No arguments expected" << endl;
		exit(EXIT_FAILURE);
	}

	// Initialise GL, GLU and GLUT
	glutInit(&argc, argv);

	// Setting up the display
	// - RGB color model + Alpha Channel = GLUT_RGBA
	// - Double buffered = GLUT_DOUBLE
	// - Depth buffer = GLUT_DEPTH
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	// Initialise window size and create window
	glutInitWindowSize(g_winWidth, g_winHeight);
	g_mainWindow = glutCreateWindow("COMP308 Assignment 3");


	// Initilise GLEW
	// must be done after creating GL context (glutCreateWindow in this case)
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}

	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;



	// Register functions for callback
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);

	glutMouseFunc(mouseCallback);
	glutPassiveMotionFunc(mouseMotionCallback);

	initLight();
	initShader();
	//initTexture();
	initCubeMap();

	// Create the scene including the geometry.
	g_scene = new Scene(g_shader);
	
	// Loop required by GLUT
	// This will not return until we tell GLUT to finish
	glutMainLoop();

	// Don't forget to delete all pointers that we made
	delete g_scene;
	return 0;
}
