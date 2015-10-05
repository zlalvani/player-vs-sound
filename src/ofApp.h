#pragma once

#include "ofMain.h"
#include "Shield.h"
#include "Obstacle.h"
#include "ofxFFTLive.h"
#include "ofxBullet.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void onCollision(ofxBulletCollisionData &cdata);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofSoundPlayer music;
		float volume; 
		ofxFFTLive fftLive;
		float avgSound;
		float* fftSmoothed;
		int nBands;
		
		ofxBulletWorldRigid world;
		Shield* shield;

		btBoxShape* boxShape;
		btSphereShape* sphereShape;

		//list<ofxBulletRigidBody*> obstacles;
		list<Obstacle> obstacles;

		ofxBulletRigidBody* player;

		ofxBulletBox* ground;
		vector<ofxBulletBox*> bounds;
		float boundsWidth;

		vector<bool> bColliding;

		ofCamera camera;
};
