#pragma once

#include "ofMain.h"
#include "Shield.h"
#include "Obstacle.h"
#include "ofxFFTLive.h"
#include "ofxBullet.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfColor.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxOpenCv.h"
#include "OfxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		float getRollingAverage(list<float> &values, float value);
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

		ofSoundPlayer			music;
		float					volume; 
		ofxFFTLive				fftLive;
		float					avgSound;
		float*					fftSmoothed;
		int						nBands;
		list<float>*			spectrumAverages;
		int						smoothAmount;
		
		ofxBulletWorldRigid		world;
		ofLight					light;

		Shield*					shield;

		ofColor					backgroundColor;
		ofxAnimatableOfColor	colorAnim;

		ofColor					playerColor;
		ofxAnimatableOfColor	playerAnim;

		ofxAnimatableFloat		speed;

		ofxAnimatableOfPoint	faceAnim;

		list<Obstacle>			obstacles;

		btBoxShape*				boxShape;
		btSphereShape*			sphereShape;

		ofxBulletBox*			player;

		ofxBulletBox*			ground;
		vector<ofxBulletBox*>	bounds;
		float					boundsWidth;

		vector<bool>			bColliding;

		ofCamera				camera;

		int						vidWidth, vidHeight;

		ofVideoGrabber			vidGrabber;
		ofxCvColorImage			colorImg;
		ofxCvGrayscaleImage		grayImg;
		ofxCvHaarFinder			faceFinder;

		ofxPanel				gui;
		ofxToggle				enableFace;
		bool					hideGui;
		ofxToggle				paused;
		//ofxToggle				newSong;
		bool					newSong;
		string					path;
		ofFileDialogResult		res;
};
