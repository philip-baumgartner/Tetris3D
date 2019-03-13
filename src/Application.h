//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "Tetromino.h"
#include "PlayingField.h"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime, bool allKeysReleased);
    void draw();
    void end();

protected:
	void getSteer(float &ForwardBackward, float &LeftRight);
	void getTurn(bool &turnLeft, bool &turnDown);
	bool checkSteeringCollision(float ForwardBackward, float LeftRight);
	bool checkTurningCollision(bool turnLeft, bool turnDown);
	bool checkFallingCollision();
	void createScene();

    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;
	BaseModel* pModel;
	Tetromino *t;
	PlayingField *p;
	ConstantShader* pConstShader;
	PhongShader *phongL, *phongS, *phongO, *phongI, *phongField;
};

#endif /* Application_hpp */
