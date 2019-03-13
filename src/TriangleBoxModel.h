//
//  TriangleBoxModel.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef TriangleBoxModel_hpp
#define TriangleBoxModel_hpp

#include <stdio.h>

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class TriangleBoxModel : public BaseModel
{
public:
	TriangleBoxModel(float Width = 1, float Height = 1, float Depth = 1);
	virtual ~TriangleBoxModel() {}
	virtual void draw(const BaseCamera& Cam);
	void update(float dtime);
	void updateFall();
	void steer(float forBack, float leftRight);

	const Vector &getPosition() const { return position; }
	void setPosition(const Vector &p) { position = p; }
protected:
	float m_forBack;
	float m_leftRight;
	Vector position;
	VertexBuffer VB;
	IndexBuffer IB;
};


#endif /* TriangleBoxModel_hpp */
