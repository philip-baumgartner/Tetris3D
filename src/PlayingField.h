#pragma once
#include "TrianglePlaneModel.h"
#include "TriangleBoxModel.h"
#include "PhongShader.h"
#include "BaseModel.h"

class PlayingField : public BaseModel {
	int width;
	int depth;
	int height;
	bool gameOver;
	TrianglePlaneModel *fieldGround;
	TriangleBoxModel **fieldLayers;
public:
	PlayingField() : width(0), depth(0), height(0), gameOver(false){}
	PlayingField(PhongShader *p, unsigned int w, unsigned int d, unsigned int h);
	~PlayingField();

	const bool GameOver() const { return gameOver; }
	void GameOver(bool g) { gameOver = g; }

	virtual void draw(const BaseCamera& Cam);
	void addCube(TriangleBoxModel *cube);
	bool positionAvialable(float x, float y, float z);
	void deleteFullLayer();
};