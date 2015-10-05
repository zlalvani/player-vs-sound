#include "Shield.h"

Shield::Shield(){
	position = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	velocity = ofVec3f(ofRandom(-5, 5), ofRandom(-5, 5), ofRandom(-5, 5));
	acceleration = ofVec3f(0);
	mesh = ofMesh::box(100, 100, 100, 1, 1, 1);
	rotatex = ofRandom(-.5, .5);
	rotatey = ofRandom(-.5, .5);
	rotatez = ofRandom(-.5, .5);
}
Shield::Shield(ofVec3f startPos, ofColor color){
	position = startPos;
	faceColor = color;
	mesh = ofMesh::box(1000, 1000, 10, 1, 1, 1);
	/*
	velocity = startVel;
	acceleration = velocity * startAccel;
	mesh = ofMesh::box(100, 100, 100, 1, 1, 1);
	rotatex = ofRandom(-.5, .5);
	rotatey = ofRandom(-.5, .5);
	rotatez = ofRandom(-.5, .5);
	*/
}

void Shield::update(float* spectrum){
	faceColor = ofColor(spectrum[3] * 16, 0, 0);
	//cout << spectrum[1] << endl;
	/*
	velocity += acceleration;
	velocity.limit(40);
	position += velocity;
	*/
}

void Shield::draw(){
	ofPushMatrix();

	ofTranslate(position);
	//ofRotate(ofGetFrameNum(), rotatex, rotatey, rotatez);
	ofSetColor(faceColor);
	mesh.draw();
	//ofSetColor(ofColor::limeGreen);
	//mesh.drawWireframe();

	ofPopMatrix();

}