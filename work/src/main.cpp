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
bool g_useShader = false;

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
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
}



void initTexture() {
	image tex("../work/res/textures/brick.jpg");

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_texture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, g_texture); // Bind it as a 2D texture
	
	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());

	cout << tex.w << endl;
}




void initShader() {
	g_shader = makeShaderProgram("../work/res/shaders/my_phong.vert", "../work/res/shaders/my_phong_multi.frag");
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




// Draw function
//
void draw() {



	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	

	setUpCamera();

	///////////////////// Lighting ////////////////////////
	float direction[]	  = {0.0f, 0.0f, 1.0f, 0.0f};
	float diffintensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float ambient[]       = {0.2f, 0.2f, 0.2f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);	

	// Weak ambient light
	float ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	// Weak directional light
	float directionalDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	float directionalSpecular[] = {0.8f, 0.8f, 0.8f, 1.0f};
	float directionalAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	float directionalPos[] = {0.5f, 0.5f, 0.5f, 0.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, directionalAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, directionalSpecular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, directionalDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, directionalPos);
	
	// Weak point light
	float pointDiffuse[] = {0.4f, 0.4f, 0.4f, 1.0f};
	float pointSpecular[] = {0.6f, 0.6f, 0.6f, 1.0f};
	float pointPos[] = {-4.0, 3.0, 0.0, 1.0f};
	GLfloat pointExponent = 8.0; 

	glLightfv(GL_LIGHT2, GL_DIFFUSE, pointDiffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, pointSpecular);
	glLightfv(GL_LIGHT2, GL_POSITION, pointPos);
	glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, &pointExponent);


	// Strong spotlight
	float spotDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float spotSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};
	float spotPos[] = {spot_x, spot_y, spot_z, 1.0f};
	float spotDir[] = {spot_dirX, spot_dirY, spot_dirZ};
	GLfloat spotCutOff = spot_cut; 
	GLfloat spotExponent = 1.0; 

	glLightfv(GL_LIGHT3, GL_DIFFUSE, spotDiffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, spotSpecular);
	glLightfv(GL_LIGHT3, GL_POSITION, spotPos);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, &spotCutOff);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDir);
	glLightfv(GL_LIGHT3, GL_SPOT_EXPONENT, &spotExponent);





	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);





	// Without shaders
	//
	if (!g_useShader) {

		// Texture setup
		//

		// Enable Drawing texures
		// glEnable(GL_TEXTURE_2D);
		// // Use Texture as the color
		// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		// // Set the location for binding the texture
		// glActiveTexture(GL_TEXTURE0);
		// // Bind the texture
		// glBindTexture(GL_TEXTURE_2D, g_texture);

		// Render a single square as our geometry
		// You would normally render your geometry here
		// glBegin(GL_QUADS);
		// glNormal3f(0.0, 0.0, 1.0);
		// glTexCoord2f(0.0, 0.0);
		// glVertex3f(-5.0, -5.0, 0.0);
		// glTexCoord2f(0.0, 1.0);
		// glVertex3f(-5.0, 5.0, 0.0);
		// glTexCoord2f(1.0, 1.0);
		// glVertex3f(5.0, 5.0, 0.0);
		// glTexCoord2f(1.0, 0.0);
		// glVertex3f(5.0, -5.0, 0.0);
		// glEnd();
		// glFlush();


		g_scene->renderScene();



	// With shaders (no lighting)
	//
	} else {

		// Texture setup
		//

		// // Enable Drawing texures
		// glEnable(GL_TEXTURE_2D);
		// // Set the location for binding the texture
		// glActiveTexture(GL_TEXTURE0);
		// // Bind the texture
		// glBindTexture(GL_TEXTURE_2D, g_texture);

		// Use the shader we made
		glUseProgram(g_shader);

		// Set our sampler (texture0) to use GL_TEXTURE0 as the source
		glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);


		// Render a single square as our geometry
		// You would normally render your geometry here
		// glBegin(GL_QUADS);
		// glNormal3f(0.0, 0.0, 1.0);
		// glTexCoord2f(0.0, 0.0);
		// glVertex3f(-5.0, -5.0, 0.0);
		// glTexCoord2f(0.0, 1.0);
		// glVertex3f(-5.0, 5.0, 0.0);
		// glTexCoord2f(1.0, 1.0);
		// glVertex3f(5.0, 5.0, 0.0);
		// glTexCoord2f(1.0, 0.0);
		// glVertex3f(5.0, -5.0, 0.0);
		// glEnd();
		// glFlush();

		g_scene->renderScene();

		// Unbind our shader
		glUseProgram(0);




	}
	float coneHeight = 2.0;

	glEnable(GL_COLOR_MATERIAL);
	// Represent spot light with OpenGL primatives
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	glColor3f(0.6,0.6,0.6); // Grey

	glPushMatrix(); 

		// Tranlate to the position of the spotlight and minus the height of the cone
		glTranslatef(spot_x,spot_y - (coneHeight),spot_z);

		// Orientate the cone
		//glRotatef(-90,1.0,0.0,0.0);



		// Change where the cone points
		vec3 p(0.0, 0.0, -1.0);
		vec3 d(spotDir[0],spotDir[1],spotDir[2]);

		double omega = acos(dot(p, d) / length(d));

		vec3 a = cross(p, d);

		glRotatef(degrees(omega),a.x, a.y, a.z);

		// Compute the radius using the height and the cut off angle
		float radius = tan(spot_cut * PI /180.00) * coneHeight;

		// Draw the cone
		glutWireCone(radius, coneHeight, 32, 8);

	glPopMatrix();



	// Disable flags for cleanup (optional)
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);

	

	

	// Move the buffer we just drew to the front
	glutSwapBuffers();

	// Queue the next frame to be drawn straight away
	glutPostRedisplay();
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
	//cout << "Keyboard Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	switch(key){

		case 't':
			g_scene->rotate();
			break;

		case 'q':
			spot_cut -= 0.4;

			if(spot_cut < 0.0){
				spot_cut = 0.0;
			}
			break;

		case 'e':
			spot_cut += 0.4;

			if(spot_cut > 90.0){
				spot_cut = 90.0;
			}
			break;
		case 'z':
			dir_trigger = !dir_trigger;
			break;

		case 'r':
			spot_y += 0.1;
			break;

		case 'f':
			spot_y -= 0.1;
			break;

		}

		// Change spot direction
		if(dir_trigger){

			switch(key){

				case 'w':
					spot_dirZ -= 0.05;
					break;
				case 'a':
					spot_dirX -= 0.05;
					break;
				case 's':
					spot_dirZ += 0.05;
					break;
				case 'd':
					spot_dirX += 0.05;
					break;
			}
		}
		// Change spot postion
		else{

			switch(key){

				case 'w':
					spot_z -= 0.1;
					break;
				case 'a':
					spot_x -= 0.1;
					break;
				case 's':
					spot_z += 0.1;
					break;
				case 'd':
					spot_x += 0.1;
					break;
			}

		}

}


// Special Keyboard callback
// Called once per button state change
//
void specialCallback(int key, int x, int y) {
	//cout << "Special Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
}


// Mouse Button Callback function
// Called once per button state change
// 
void mouseCallback(int button, int state, int x, int y) {
	//cout << "Mouse Callback :: button=" << button << ", state=" << state << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	switch(button){

		case 0: // left mouse button
			g_mouseDown = (state==0);
			g_mousePos = vec2(x, y);
			break;

		case 2: // right mouse button
			if (state==0)
				g_useShader = !g_useShader;
			break;

		case 3: // scroll foward/up
			g_zoomFactor /= 1.1;
			break;

		case 4: // scroll back/down
			g_zoomFactor *= 1.1;
			break;
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
	if (g_mouseDown) {
		vec2 dif = vec2(x,y) - g_mousePos;
		g_mousePos = vec2(x,y);
		g_yRotation += 0.3 * dif.x;
		g_xRotation += 0.3 * dif.y;
	}
}



//Main program
// 
int main(int argc, char **argv) {

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
	glutMotionFunc(mouseMotionCallback);

	initLight();
	initShader();
	initTexture();

	// Create the scene including the geometry.
	g_scene = new Scene(g_shader);
	

	// Loop required by GLUT
	// This will not return until we tell GLUT to finish
	glutMainLoop();

	// Don't forget to delete all pointers that we made
	delete g_scene;
	return 0;
}