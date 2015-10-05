#include "Obstacle.h"

Obstacle::Obstacle() {
	body = new ofxBulletSphere();
	age = 0;
}

Obstacle::~Obstacle(){
	//delete body;
}

void Obstacle::update(){
	//body->update();
	age++;
}