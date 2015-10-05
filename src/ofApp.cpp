#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofEnableDepthTest(); //enable z-buffering

	camera.setPosition(ofVec3f(0, 0, -40.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	world.setup();

	world.enableGrabbing();
	//ofAddListener(world.MOUSE_PICK_EVENT, this, &ofApp::mousePickEvent);

	world.enableCollisionEvents();
	ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
	world.setCamera(&camera);
	world.setGravity(ofVec3f(0, 0, -12));
	light.setPosition(camera.getPosition().x, camera.getPosition().y - 14, camera.getPosition().z);

	volume = 1.0;

	music.loadSound("test.mp3");
	music.setVolume(volume);
	music.play();
	music.setLoop(true);

	fftLive.setMirrorData(false);
	fftLive.setup();

	fftSmoothed = new float[8192];
	memset(fftSmoothed, 0, sizeof(float) * 8192);
	nBands = 16;

	boxShape = ofBtGetBoxCollisionShape(2.65, 2.65, 2.65);
	sphereShape = ofBtGetSphereCollisionShape(.2);
	/*
	ofVec3f groundLoc;
	ofPoint dimens;

	ground = new ofxBulletBox();

	groundLoc.set(0.0, 16.0, 0.0);
	dimens.set(30.0, 2.0, 30.0);
	cout << "first check" << endl;
	ground->create(world.world, groundLoc, 0.0, dimens.x, dimens.y, dimens.z);
	cout << "ground created" << endl;
	ground->setProperties(.25, .95);
	ground->add();
	cout << "ground added" << endl;
	*/
	ofVec3f startLoc;
	ofPoint dimens;
	boundsWidth = 30.;
	float hwidth = boundsWidth*.5;
	float depth = 2.;
	float hdepth = depth*.5;

	for (int i = 0; i < 5; i++) {
		bounds.push_back(new ofxBulletBox());
		if (i == 0) { // ground //
			startLoc.set(0., hwidth + hdepth, 0.);
			dimens.set(boundsWidth, depth, boundsWidth);
		}
		else if (i == 1) { // back wall //
			startLoc.set(0, 0, hwidth + hdepth);
			dimens.set(boundsWidth, boundsWidth, depth);
		}
		else if (i == 2) { // right wall //
			startLoc.set(hwidth + hdepth, 0, 0.);
			dimens.set(depth, boundsWidth, boundsWidth);
		}
		else if (i == 3) { // left wall //
			startLoc.set(-hwidth - hdepth, 0, 0.);
			dimens.set(depth, boundsWidth, boundsWidth);
		}
		else if (i == 4) { // ceiling //
			startLoc.set(0, -hwidth - hdepth, 0.);
			dimens.set(boundsWidth, depth, boundsWidth);
		}

		bounds[i]->create(world.world, startLoc, 0., dimens.x, dimens.y, dimens.z);
		bounds[i]->setProperties(.25, .95);
		bounds[i]->add();
	}
	player = new ofxBulletBox();

	player->create(world.world, ofVec3f(0, 0, -30), 0, 3, 3, 1);
	player->setProperties(.25, .2);
	player->add();

	backgroundColor = ofColor(ofColor::black);

	ofBackground(backgroundColor);

	shield = new Shield(ofVec3f(0, 0, 0), ofColor::black);
}

//--------------------------------------------------------------
void ofApp::update(){
	

	ofSoundUpdate();
	float* spectrum = ofSoundGetSpectrum(nBands);
	shield->update(spectrum);

	for (int i = 0; i < 2; i++){
		if (spectrum[i] > .6){
			obstacles.push_back(Obstacle());
			ofxBulletRigidBody* temp_shape = obstacles.back().body;
			((ofxBulletSphere*)temp_shape)->init(sphereShape);
			((ofxBulletSphere*)temp_shape)->create(world.world, 
				ofVec3f(ofRandom(-20, 20), ofRandom(-20, 20), -5), 2.0, .4);
			//temp_shape->setActivationState(DISABLE_DEACTIVATION);
			temp_shape->add();
			break;
		}
	}


	vector<list<Obstacle>::iterator> itr_vec;
	for (list<Obstacle>::iterator itr = obstacles.begin();
	itr != obstacles.end(); itr++) {
		itr->update();
		if (itr->age >= 400) {
			itr_vec.push_back(itr);
		}
	}

	for (unsigned int i = 0; i < itr_vec.size(); i++) {
		itr_vec[i]->body->remove();
		obstacles.erase(itr_vec[i]);
	}

	backgroundColor = ofColor(floor(spectrum[0] * 16) * 16, 0, 0);

	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	glEnable(GL_DEPTH_TEST);
	camera.begin();
	shield->draw();

	//ofSetColor(ofColor::darkGrey);

	ofEnableLighting();
	light.enable();
	ofBackground(backgroundColor);
	//ground->draw();

	ofSetColor(ofColor::darkCyan);
	for (list<Obstacle>::iterator itr = obstacles.begin();
	itr != obstacles.end(); itr++) {
		//cout << "drawing" << endl;
		itr->body->draw();
	}
	ofSetColor(ofColor::blueViolet, 50);
	player->draw();

	light.disable();
	ofDisableLighting();

	camera.end();
	glDisable(GL_DEPTH_TEST);
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
void ofApp::onCollision(ofxBulletCollisionData& cdata){
	//cout << "collision" << endl;
	/*
	if (*ground == cdata) {
		cout << "deleting" << endl;
		for (list<ofxBulletRigidBody*>::iterator itr = obstacles.begin();
		itr != obstacles.end(); itr++) {
			//delete *itr;	
		}
		//obstacles.clear();
	}
	*/
}

/*
void mousePickEvent(ofxBulletMousePickEvent &e) {

}
*/

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
	ofVec3f mouseLoc = camera.screenToWorld(ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0));
	player->remove();
	player->create(world.world, mouseLoc, 0, 3, 3, 1);
	player->setProperties(.25, .95);
	player->add();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofVec3f mouseLoc = camera.screenToWorld(ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0));
	player->remove();
	player->create(world.world, mouseLoc, 0, 3, 3, 1);
	player->setProperties(.25, .95);
	player->add();
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
