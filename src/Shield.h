#pragma once

#include "ofMain.h"

class Shield {

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	float rotatex;
	float rotatey;
	float rotatez;
	ofMesh mesh;

	ofColor faceColor;
	ofColor lineColor;

public:
	Shield();
	Shield(ofVec3f startPos, ofColor color);
	void update(float* spectrum);
	void draw();
	
};