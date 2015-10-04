#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofEnableDepthTest(); //enable z-buffering

	camera.setPosition(ofVec3f(0, -4.f, -40.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	world.setup();
	world.enableCollisionEvents();
	ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
	world.setCamera(&camera);
	world.setGravity(ofVec3f(0, 25, 0));

	volume = 1.0;

	music.loadSound("test.mp3");
	music.setVolume(volume);
	music.play();
	music.setLoop(true);

	fftLive.setMirrorData(false);
	fftLive.setup();

	fftSmoothed = new float[8192];
	memset(fftSmoothed, 0, sizeof(float) * 8192);
	nBands = 128;

	boxShape = ofBtGetBoxCollisionShape(2.65, 2.65, 2.65);
	sphereShape = ofBtGetSphereCollisionShape(2.5);

	ofVec3f groundLoc;
	ofPoint dimens;

	ground = new ofxBulletBox();

	groundLoc.set(0.0, 16.0, 0.0);
	dimens.set(30.0, 2.0, 30.0);
	cout << "first check" << endl;
	//ground->init()
	ground->create(world.world, groundLoc, 0.0, dimens.x, dimens.y, dimens.z);
	cout << "ground created" << endl;
	ground->setProperties(.25, .95);
	ground->add();
	cout << "test" << endl;
	shield = new Shield(ofVec3f(1000, 1000, -100), ofColor::aqua);
}

//--------------------------------------------------------------
void ofApp::update(){
	shield->update();

	ofSoundUpdate();
	float* spectrum = ofSoundGetSpectrum(nBands);
	for (int i = 0; i < nBands; i++){
		if (spectrum[i] > .3){
			obstacles.push_back(new ofxBulletSphere());
			ofxBulletRigidBody* temp_shape = obstacles.back();
			((ofxBulletSphere*)temp_shape)->init(sphereShape);
			((ofxBulletSphere*)temp_shape)->create(world.world, 
				ofVec3f(ofRandom(-3, 3), ofRandom(-2, 2), ofRandom(-1, 1)), 0.2);
			temp_shape->setActivationState(DISABLE_DEACTIVATION);
			temp_shape->add();
			break;
		}
	}


	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	camera.begin();
	shield->draw();

	ground->draw();

	for (list<ofxBulletRigidBody*>::iterator itr = obstacles.begin();
	itr != obstacles.end(); itr++) {
		(*itr)->draw();
	}
	camera.end();
	ofPushMatrix();

	//ofTranslate(0, 0, 0);

	ofPopMatrix();
	/*
	ofSetColor(255, 255, 255);
	stringstream ss;
	ss << "framerate: " << ofToString(ofGetFrameRate(), 0) << endl;
	ss << "num shapes: " << (shapes.size() + bounds.size()) << endl;
	ss << "draw debug (d): " << ofToString(bDrawDebug, 0) << endl;
	ss << "render shapes (r): " << ofToString(bRenderShapes, 0) << endl;
	ss << "mouse force with spacebar: " << bSpacebar << endl;
	ss << "force direction(f): " << bAddCenterAttract << endl;
	ss << "add spherers (s)" << endl;
	ss << "add boxes (b)" << endl;
	ofDrawBitmapString(ss.str().c_str(), 10, 10);
	*/
}

//--------------------------------------------------------------
void ofApp::onCollision(ofxBulletCollisionData & data){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
