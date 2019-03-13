#pragma once

#ifndef LineModel_hpp
#define LineModel_hpp

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"

class LineModel : public BaseModel
{
public:
	LineModel(Vector position, float height);
	virtual ~LineModel() {}
	virtual void draw(const BaseCamera& Cam);
	void update(float dtime);
	void updateFall();
	void steer(float forBack, float leftRight);
protected:
	float m_forBack;
	float m_leftRight;
	VertexBuffer VB;
	Vector position;
};

#endif /* LineModel_hpp */