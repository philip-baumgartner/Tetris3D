#include "LineModel.h"

LineModel::LineModel(Vector position, float height): position(position) {
	VB.begin();

	VB.addVertex(position.X, 0, position.Z);
	VB.addVertex(position.X, height, position.Z);

	VB.end();
}

void LineModel::draw(const BaseCamera& Cam) {
	BaseModel::draw(Cam);

	VB.activate();
	glDrawArrays(GL_LINES, 0, VB.vertexCount());
	VB.deactivate();
}

void LineModel::update(float dtime) {
	Matrix LineMat = this->transform();
	Matrix forBackMat, leftRightMat;

	forBackMat.translation(0, 0, m_forBack * 1);
	leftRightMat.translation(m_leftRight * 1, 0, 0);

	LineMat = LineMat * forBackMat * leftRightMat;
	this->transform(LineMat);

	position.Z += m_forBack;
	position.X += m_leftRight;
}

void LineModel::updateFall() {
	// Notwendig???
}

void LineModel::steer(float forBack, float leftRight) {
	m_forBack = forBack;
	m_leftRight = leftRight;
}