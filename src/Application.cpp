//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "triangleboxmodel.h"
#include "LineModel.h"
#include "model.h"
#include "ShaderLightmapper.h"
#include <time.h>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

#define FIELD_DIMENSION 6

float passedTime = 0;
int timeCounter = 0;

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), t(NULL){
	// PhongShader
	phongField = new PhongShader();
	phongField->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "field2.png"));

	phongL = new PhongShader();
	phongL->ambientColor(Color(0.2f, 0.f, 0.f));
	phongL->diffuseColor(Color(1.0f, 0.f, 0.f));
	phongL->specularColor(Color(1.0f, 0.f, 0.f));
	phongL->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "stone.png"));

	phongS = new PhongShader();
	phongS->ambientColor(Color(0.f, 0.2f, 0.f));
	phongS->diffuseColor(Color(0.f, 1.0f, 0.f));
	phongS->specularColor(Color(0.f, 1.0f, 0.f));
	phongS->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "stone.png"));

	phongO = new PhongShader();
	phongO->ambientColor(Color(0.2f, 0.2f, 0.f));
	phongO->diffuseColor(Color(1.0f, 1.0f, 0.f));
	phongO->specularColor(Color(1.0f, 1.0f, 0.f));
	phongO->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "stone.png"));

	phongI = new PhongShader();
	phongI->ambientColor(Color(0.f, 0.f, 0.2f));
	phongI->diffuseColor(Color(0.f, 0.f, 1.0f));
	phongI->specularColor(Color(0.f, 0.f, 1.0f));
	phongI->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "stone.png"));

	//ConstShader
	pConstShader = new ConstantShader();
	pConstShader->color(Color(1, 1, 1));

	p = new PlayingField(phongField, FIELD_DIMENSION, FIELD_DIMENSION, FIELD_DIMENSION);
	Models.push_back(p);

	t = new TetrominoO();
	t->shader(phongO, true);
	t->constShader(pConstShader, false);
	t->generate(FIELD_DIMENSION);
	Models.push_back(t);

	createScene();
}

void Application::start(){
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime, bool allKeysReleased){
	passedTime += dtime;
	float ForwardBackward, LeftRight = 0;
	bool turnLeft, turnDown = false;

	if (!t->getFinalPosition() && !p->GameOver()) {
		// Automatische Bewegung nach unten
		if (passedTime >= timeCounter) {
			t->updateFall();
			timeCounter++;

			if (!checkFallingCollision()) {
				t->setFinalPosition(true);
				t->deleteLines();
			}	
		}

		// Steuerung rechts/links/vor/zurück
		if (allKeysReleased) {
			getSteer(ForwardBackward, LeftRight);
			getTurn(turnLeft, turnDown);

			if (checkSteeringCollision(ForwardBackward, LeftRight)) {
				t->steer(ForwardBackward, LeftRight);
				t->update(dtime);
			}
		}
	}
	else if(!p->GameOver()) {
		// Add cubes of old tetromino to playingfield and create a new current tetromino
		for (unsigned int i = 0; i < t->num; i++) {
			p->addCube(t->getCube(i));
		}
		p->deleteFullLayer();
		Models.remove(t);

		time_t ti;
		time(&ti);
		srand((unsigned int)ti);
		int number = rand() % 4;

		switch (number) {
		case 0:
			t = new TetrominoL();
			t->shader(phongL, true);
			break;
		case 1:
			t = new TetrominoS();
			t->shader(phongS, true);
			break;
		case 2:
			t = new TetrominoO();
			t->shader(phongO, true);
			break;
		case 3:
			t = new TetrominoI();
			t->shader(phongI, true);
			break;
		}
		t->constShader(pConstShader, false);
		t->generate(FIELD_DIMENSION);
		Models.push_back(t);
		
		// Game over
		if (!checkFallingCollision()) {
			t->setFinalPosition(true);
			t->deleteLines();
			p->GameOver(true);
		}
	}
	
    Cam.update();
}

void Application::draw(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
	ShaderLightMapper::instance().deactivate();
	
    GLenum Error = glGetError();
    assert(Error==0);
}

void Application::end(){
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();
}

void Application::createScene(){
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pModel->shader(new PhongShader(), true);
	pModel->shadowCaster(false);
	Models.push_back(pModel);

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0, -1, 0));
	dl->color(Color(0.25, 0.25, 0.25));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);
	
	Color c = Color(0.25f, 0.25f, 0.25f);
	Vector a = Vector(0, 0, 0.05f);
	float innerradius = 18;
	float outerradius = 25;

	SpotLight* sl = new SpotLight();
	sl->position(Vector(-7, 8, 0));
	sl->color(c);
	sl->direction(Vector(3, -8, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);

	sl = new SpotLight();
	sl->position(Vector(7, 8, 0));
	sl->color(c);
	sl->direction(Vector(-3, -8, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);

	PointLight* pl = new PointLight();
	pl->position(Vector(0, 4, 5));
	pl->color(c);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
}

void Application::getSteer(float &ForwardBackward, float &LeftRight) {
	LeftRight = 0;
	ForwardBackward = 0;
	bool UpPressed = glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS;
	bool DownPressed = glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS;
	bool LeftPressed = glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS;
	bool RightPressed = glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS;

	if (UpPressed)
		ForwardBackward = -1;
	else if (DownPressed)
		ForwardBackward = 1;
	if (LeftPressed)
		LeftRight = -1;
	else if (RightPressed)
		LeftRight = 1;
}

void Application::getTurn(bool &turnLeft, bool &turnDown) {
	turnLeft = 0;
	turnDown = 0;
	turnLeft = glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS;
	turnDown = glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS;
}

bool Application::checkSteeringCollision(float ForwardBackward, float LeftRight) {
	// No steering
	if (ForwardBackward == 0 && LeftRight == 0)
		return false;
	
	float borders = FIELD_DIMENSION / 2 - 0.5;

	for (unsigned int i = 0; i < t->num; i++) {
		// Check for collisions with the borders of the playing field
		if(ForwardBackward == 1 && t->getPosition(i)->Z == borders ||
			ForwardBackward == -1 && t->getPosition(i)->Z == -borders || 
			LeftRight == 1 && t->getPosition(i)->X == borders || 
			LeftRight == -1 && t->getPosition(i)->X == -borders)
			return false;
		// Check for collisions with other tetrominos
		if (ForwardBackward == 1 && !p->positionAvialable(t->getPosition(i)->X, t->getPosition(i)->Y, t->getPosition(i)->Z + 1) ||
			ForwardBackward == -1 && !p->positionAvialable(t->getPosition(i)->X, t->getPosition(i)->Y, t->getPosition(i)->Z - 1) ||
			LeftRight == 1 && !p->positionAvialable(t->getPosition(i)->X + 1, t->getPosition(i)->Y, t->getPosition(i)->Z) ||
			LeftRight == -1 && !p->positionAvialable(t->getPosition(i)->X - 1, t->getPosition(i)->Y, t->getPosition(i)->Z))
			return false;
	}
	return true;
}

bool Application::checkTurningCollision(bool turnLeft, bool turnDown) {
	// No turning
	if (!turnLeft && !turnDown)
		return false;

	// TODO:Check for collisions with the borders of the playing field

	// TODO:Check for collisions with other tetrominos
	return true;
}

bool Application::checkFallingCollision() {
	for (unsigned int i = 0; i < t->num; i++) {
		// Check for collisions with the borders of the playing field
		if (t->getPosition(i)->Y == 0.5) 
			return false;
		// Check for collisions with other tetrominos
		if (!p->positionAvialable(t->getPosition(i)->X, t->getPosition(i)->Y - 1, t->getPosition(i)->Z))
			return false;
	}
	return true;
}