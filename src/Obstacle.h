#pragma once

#include "ofMain.h"
#include "ofxBullet.h"

class Obstacle {
	
public:
	Obstacle();
	~Obstacle();
	void update();
	unsigned int age;
	ofxBulletRigidBody* body;

};