#include "Tetromino.h"

Tetromino::Tetromino(): lineShader(NULL), deleteShader(false), gotFinalPosition(false){
	for (int i = 0; i < NUMBER; i++) {
		positions[i] = NULL;
		cubes[i] = NULL;
		lines[i] = NULL;
	}
}

Tetromino::~Tetromino(){
	for (int i = 0; i < NUMBER; i++) {
		delete positions[i];
		delete cubes[i];
	}
	if (deleteShader && lineShader)
		delete lineShader;
	deleteShader = false;
	lineShader = NULL;
}

void Tetromino::constShader(ConstantShader *constShader, bool deleteOnDestruction) {
	lineShader = constShader;
	deleteShader = deleteOnDestruction;
}

void Tetromino::update(float dtime) {
	for (int i = 0; i < NUMBER; i++) {
		cubes[i]->update(dtime);
		if(!gotFinalPosition)
			lines[i]->update(dtime);
	}
}

void Tetromino::steer(float forBack, float leftRight) {
	for (int i = 0; i < NUMBER; i++) {
		positions[i]->Z += forBack;
		positions[i]->X += leftRight;
		cubes[i]->steer(forBack, leftRight);
		if (!gotFinalPosition)
			lines[i]->steer(forBack, leftRight);
	}
}

void Tetromino::updateFall() {
	for (int i = 0; i < NUMBER; i++) {
		positions[i]->Y -= 1;
		cubes[i]->updateFall();
		if (!gotFinalPosition)
			lines[i]->updateFall();
	}
}

void Tetromino::draw(const BaseCamera& Cam) {
	for (int i = 0; i < NUMBER; i++) {
		cubes[i]->draw(Cam);
		if (!gotFinalPosition)
			lines[i]->draw(Cam);
	}
}

void Tetromino::deleteLines() {
	for (int i = 0; i < NUMBER; i++) {
		delete lines[i];
	}
}

void TetrominoI::generate(int pfHeight) {
	positions[0] = new Vector(-1.5, pfHeight - 0.5, 0.5);
	positions[1] = new Vector(-0.5, pfHeight - 0.5, 0.5);
	positions[2] = new Vector(0.5, pfHeight - 0.5, 0.5);
	positions[3] = new Vector(1.5, pfHeight - 0.5, 0.5);

	Matrix cubeMat, transMat;

	for (int i = 0; i < NUMBER; i++) {
		cubes[i] = new TriangleBoxModel(1, 1, 1);
		lines[i] = new LineModel(*positions[i], 9.5);
		cubes[i]->shader(shader());
		lines[i]->shader(constShader());

		// Initiale Positionierung
		cubeMat = cubes[i]->transform();
		transMat.translation(positions[i]->X, positions[i]->Y, positions[i]->Z);
		cubeMat = cubeMat * transMat;
		cubes[i]->transform(cubeMat);

		cubes[i]->setPosition(*positions[i]);
	}
}

void TetrominoL::generate(int pfHeight) {
	positions[0] = new Vector(-0.5, pfHeight - 0.5, 0.5);
	positions[1] = new Vector(0.5, pfHeight - 0.5, 0.5);
	positions[2] = new Vector(1.5, pfHeight - 0.5, 0.5);
	positions[3] = new Vector(1.5, pfHeight - 0.5, -0.5);

	Matrix cubeMat, transMat;

	for (int i = 0; i < NUMBER; i++) {
		cubes[i] = new TriangleBoxModel(1, 1, 1);
		lines[i] = new LineModel(*positions[i], 9.5);
		cubes[i]->shader(shader());
		lines[i]->shader(constShader());

		// Initiale Positionierung
		cubeMat = cubes[i]->transform();
		transMat.translation(positions[i]->X, positions[i]->Y, positions[i]->Z);
		cubeMat = cubeMat * transMat;
		cubes[i]->transform(cubeMat);

		cubes[i]->setPosition(*positions[i]);
	}
}

void TetrominoO::generate(int pfHeight) {
	positions[0] = new Vector(-0.5, pfHeight - 0.5, -0.5);
	positions[1] = new Vector(0.5, pfHeight - 0.5, 0.5);
	positions[2] = new Vector(-0.5, pfHeight - 0.5, 0.5);
	positions[3] = new Vector(0.5, pfHeight - 0.5, -0.5);

	Matrix cubeMat, transMat;

	for (int i = 0; i < NUMBER; i++) {
		cubes[i] = new TriangleBoxModel(1, 1, 1);
		lines[i] = new LineModel(*positions[i], 9.5);
		cubes[i]->shader(shader());
		lines[i]->shader(constShader());

		// Initiale Positionierung
		cubeMat = cubes[i]->transform();
		transMat.translation(positions[i]->X, positions[i]->Y, positions[i]->Z);
		cubeMat = cubeMat * transMat;
		cubes[i]->transform(cubeMat);

		cubes[i]->setPosition(*positions[i]);
	}
}

void TetrominoS::generate(int pfHeight) {
	positions[0] = new Vector(-0.5, pfHeight - 0.5, -0.5);
	positions[1] = new Vector(0.5, pfHeight - 0.5, -0.5);
	positions[2] = new Vector(0.5, pfHeight - 0.5, 0.5);
	positions[3] = new Vector(1.5, pfHeight - 0.5, 0.5);

	Matrix cubeMat, transMat;

	for (int i = 0; i < NUMBER; i++) {
		cubes[i] = new TriangleBoxModel(1, 1, 1);
		lines[i] = new LineModel(*positions[i], 9.5);
		cubes[i]->shader(shader());
		lines[i]->shader(constShader());

		// Initiale Positionierung
		cubeMat = cubes[i]->transform();
		transMat.translation(positions[i]->X, positions[i]->Y, positions[i]->Z);
		cubeMat = cubeMat * transMat;
		cubes[i]->transform(cubeMat);

		cubes[i]->setPosition(*positions[i]);
	}
}
