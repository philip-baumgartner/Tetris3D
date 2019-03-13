#include "PlayingField.h"

PlayingField::PlayingField(PhongShader *p, unsigned int w, unsigned int d, unsigned int h): width(w), depth(d), height(h){
	gameOver = false;
	fieldGround = new TrianglePlaneModel(width, depth, width, depth);
	fieldGround->shader(p, true);

	fieldLayers = new TriangleBoxModel*[height];
	for (int i = 0; i < height; i++) {
		fieldLayers[i] = new TriangleBoxModel[width*depth];

		for (int j = 0; j < width*depth; j++) {
			fieldLayers[i][j] = TriangleBoxModel();
		}
	}
}

PlayingField::~PlayingField() {
	for (int i = 0; i < height; i++) {
		delete[] fieldLayers[i];
	}
	delete[] fieldLayers;
	delete fieldGround;
}

void PlayingField::draw(const BaseCamera& Cam) {
	fieldGround->draw(Cam);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width*depth; j++) {
			if (fieldLayers[i][j].shader() != NULL) {
				fieldLayers[i][j].draw(Cam);
			}
		}
	}
}

void PlayingField::addCube(TriangleBoxModel *cube) {
	int i = cube->getPosition().Y - 0.5;
	int j =(cube->getPosition().X + (width/2 - 0.5)) + (cube->getPosition().Z + (depth / 2 - 0.5)) * width;

	fieldLayers[i][j] = *cube;
}

bool PlayingField::positionAvialable(float x, float y, float z) {
	int i = y - 0.5;
	int j = (x + (width / 2 - 0.5)) + (z + (depth / 2 - 0.5)) * width;

	if (fieldLayers[i][j].shader() == NULL)
		return true;

	return false;
}

void PlayingField::deleteFullLayer() {
	bool layerIsFull = true;

	for (int i = height-1; i >= 0; i--) {
		for (int j = 0; j < width*depth; j++) {
			if (fieldLayers[i][j].shader() == NULL) {
				layerIsFull = false;
			}
		}
		if (layerIsFull) { 
			for (int j = 0; j < width*depth; j++) {
				fieldLayers[i][j] = TriangleBoxModel();
			}
		}
		layerIsFull = true;
	}
	// TODO: Obere Ebenen nach unten verschieben, falls untere Ebene gelöscht wird
}