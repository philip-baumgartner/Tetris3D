//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth): m_forBack(0), m_leftRight(0){
	float startx = Width / 2;
	float starty = Height / 2;
	float startz = Depth / 2;

	position = Vector(0, 0, 0);

	VB.begin();

	// Fläche vorne
	VB.addNormal(0, 0, 1);
	VB.addTexcoord0(0, 0);
	VB.addVertex(-startx, starty, startz);

	VB.addNormal(0, 0, 1);
	VB.addTexcoord0(0, 1);
	VB.addVertex(-startx, -starty, startz);

	VB.addNormal(0, 0, 1);
	VB.addTexcoord0(1, 0);
	VB.addVertex(startx, starty, startz);

	VB.addNormal(0, 0, 1);
	VB.addTexcoord0(1, 1);
	VB.addVertex(startx, -starty, startz);

	// Fläche hinten
	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(1, 0);
	VB.addVertex(-startx, starty, -startz);

	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(1, 1);
	VB.addVertex(-startx, -starty, -startz);

	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(0, 0);
	VB.addVertex(startx, starty, -startz);

	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(0, 1);
	VB.addVertex(startx, -starty, -startz);

	//Fläche rechts
	VB.addNormal(1, 0, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(startx, starty, startz);

	VB.addNormal(1, 0, 0);
	VB.addTexcoord0(0, 1);
	VB.addVertex(startx, -starty, startz);

	VB.addNormal(1, 0, 0);
	VB.addTexcoord0(1, 0);
	VB.addVertex(startx, starty, -startz);

	VB.addNormal(1, 0, 0);
	VB.addTexcoord0(1, 1);
	VB.addVertex(startx, -starty, -startz);

	// Fläche links
	VB.addNormal(-1, 0, 0);
	VB.addTexcoord0(1, 0);
	VB.addVertex(-startx, starty, startz);

	VB.addNormal(-1, 0, 0);
	VB.addTexcoord0(1, 1);
	VB.addVertex(-startx, -starty, startz);

	VB.addNormal(-1, 0, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(-startx, starty, -startz);

	VB.addNormal(-1, 0, 0);
	VB.addTexcoord0(0, 1);
	VB.addVertex(-startx, -starty, -startz);

	// Fläche oben
	VB.addNormal(0, 1, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(-startx, starty, -startz);

	VB.addNormal(0, 1, 0);
	VB.addTexcoord0(0, 1);
	VB.addVertex(-startx, starty, startz);

	VB.addNormal(0, 1, 0);
	VB.addTexcoord0(1, 0);
	VB.addVertex(startx, starty, -startz);

	VB.addNormal(0, 1, 0);
	VB.addTexcoord0(1, 1);
	VB.addVertex(startx, starty, startz);

	// Fläche unten
	VB.addNormal(0, -1, 0);
	VB.addTexcoord0(0, 1);
	VB.addVertex(-startx, -starty, -startz);

	VB.addNormal(0, -1, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(-startx, -starty, startz);

	VB.addNormal(0, -1, 0);
	VB.addTexcoord0(1, 1);
	VB.addVertex(startx, -starty, -startz);

	VB.addNormal(0, -1, 0);
	VB.addTexcoord0(1, 0);
	VB.addVertex(startx, -starty, startz);

	VB.end();

	IB.begin();
	// Vorne/hinten (0 - 7)
	IB.addIndex(0); IB.addIndex(1); IB.addIndex(2);
	IB.addIndex(3); IB.addIndex(2); IB.addIndex(1);
	IB.addIndex(5); IB.addIndex(6); IB.addIndex(7);
	IB.addIndex(5); IB.addIndex(4); IB.addIndex(6);
	// rechts/links (8-15)
	IB.addIndex(8);  IB.addIndex(9);  IB.addIndex(10);
	IB.addIndex(11); IB.addIndex(10); IB.addIndex(9);
	IB.addIndex(13); IB.addIndex(14); IB.addIndex(15);
	IB.addIndex(13); IB.addIndex(12); IB.addIndex(14);
	// Oben/Unten (16 - 23)
	IB.addIndex(16); IB.addIndex(17); IB.addIndex(18);
	IB.addIndex(18); IB.addIndex(17); IB.addIndex(19);
	IB.addIndex(22); IB.addIndex(21); IB.addIndex(20);
	IB.addIndex(23); IB.addIndex(21); IB.addIndex(22);

	IB.end();
}

void TriangleBoxModel::update(float dtime) {
	Matrix BoxMat = this->transform();
	Matrix forBackMat, leftRightMat;

	forBackMat.translation(0, 0, m_forBack * 1);
	leftRightMat.translation(m_leftRight * 1, 0, 0);

	BoxMat = BoxMat * forBackMat * leftRightMat;
	this->transform(BoxMat);

	position.Z += m_forBack;
	position.X += m_leftRight;
}

void TriangleBoxModel::updateFall() {
	Matrix BoxMat = this->transform();
	Matrix fallMat = fallMat.translation(0, -1, 0);

	BoxMat = BoxMat * fallMat;
	this->transform(BoxMat);

	position.Y -= 1;
}

void TriangleBoxModel::steer(float forBack, float leftRight) {
	m_forBack = forBack;
	m_leftRight = leftRight;
}

void TriangleBoxModel::draw(const BaseCamera& Cam){
	BaseModel::draw(Cam);

	VB.activate();
	IB.activate();

	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

	IB.deactivate();
	VB.deactivate();
}
