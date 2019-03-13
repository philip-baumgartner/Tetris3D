#pragma once

#include "TriangleBoxModel.h"
#include "LineModel.h"
#include "ConstantShader.h"

#define NUMBER 4

class Tetromino: public BaseModel{
protected:
	Tetromino();
	~Tetromino();

	Vector *positions[NUMBER];
	TriangleBoxModel *cubes[NUMBER];
	LineModel *lines[NUMBER];
	ConstantShader *lineShader;
	bool deleteShader;
	bool gotFinalPosition;
public:
	unsigned int num = NUMBER;

	void constShader(ConstantShader *constShader, bool deleteOnDestruction = false);
	ConstantShader* constShader() const {return lineShader; }
	void setFinalPosition(bool finalPosition) { gotFinalPosition = finalPosition; }
	const bool getFinalPosition() const { return gotFinalPosition; }
	Vector* getPosition(unsigned int i) const { return positions[i]; }
	TriangleBoxModel* getCube(unsigned int i) const { return cubes[i]; }

	virtual void generate(int pfHeight) {};
	void update(float dtime);
	void updateFall();
	void steer(float forBack, float leftRight);
	virtual void draw(const BaseCamera& Cam);
	void deleteLines();
};

class TetrominoI : public Tetromino {
public:
	virtual void generate(int pfHeight);
};

class TetrominoL : public Tetromino {
public:
	virtual void generate(int pfHeight);
};

class TetrominoO : public Tetromino {
public:
	virtual void generate(int pfHeight);
};

class TetrominoS : public Tetromino {
public:
	virtual void generate(int pfHeight);
};

