#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofEnableDepthTest(); //enable z-buffering

	camera.setPosition(ofVec3f(0, 0, -40.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	world.setup();

	world.enableGrabbing();

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

	player = new ofxBulletBox();

	player->create(world.world, ofVec3f(0, 0, 0), 0, 3, 3, 1);
	player->setProperties(.25, .2);
	player->add();

	backgroundColor = ofColor(ofColor::black);

	ofBackground(backgroundColor);

	colorAnim.setColor(ofColor::black);
	colorAnim.setDuration(0.1f);
	colorAnim.setRepeatType(LOOP);
	colorAnim.setCurve(CUBIC_BEZIER_PARAM);

	playerAnim.setColor(ofColor(ofColor::deepPink, 50));
	playerAnim.setDuration(0.1f);
	playerAnim.setRepeatType(LOOP);
	playerAnim.setCurve(CUBIC_BEZIER_PARAM);

	speed.reset(1.0);
	speed.setCurve(EASE_IN);
	speed.setRepeatType(LOOP);
	speed.setDuration(.5);

	shield = new Shield(ofVec3f(0, 0, 0), ofColor::black);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	float dt = 1.0f / 60.0f;

	ofSoundUpdate();
	float* spectrum = ofSoundGetSpectrum(nBands);
	shield->update(spectrum);

	colorAnim.update(dt);
	playerAnim.update(dt);
	speed.update(dt);
	music.setVolume(volume);
	for (int i = 0; i < 2; i++){
		if (spectrum[i] > .6){
			obstacles.push_back(Obstacle());
			ofxBulletRigidBody* temp_shape = obstacles.back().body;
			((ofxBulletSphere*)temp_shape)->init(sphereShape);
			((ofxBulletSphere*)temp_shape)->create(world.world, 
				ofVec3f(ofRandom(-15, 15), ofRandom(-15, 15), -5), 2.0, .4);
			//temp_shape->setActivationState(DISABLE_DEACTIVATION);
			temp_shape->add();
			//break;
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
	if (volume <= .999) volume += .001;
	playerColor = ofColor(255, volume * 128, volume * 128, 100 - 50 * volume);
	backgroundColor = ofColor(spectrum[0] * 255, 0, 0);
	colorAnim.animateTo(backgroundColor);
	playerAnim.animateTo(playerColor);
	speed.animateToIfFinished(spectrum[0]);
	world.setGravity(ofVec3f(0, 0, -40 * speed.val()));
	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	glEnable(GL_DEPTH_TEST);
	camera.begin();
	shield->draw();

	ofEnableLighting();
	light.enable();

	colorAnim.applyCurrentColor();
	for (list<Obstacle>::iterator itr = obstacles.begin();
	itr != obstacles.end(); itr++) {
		itr->body->draw();
	}
	playerAnim.applyCurrentColor();
	player->draw();

	light.disable();
	ofDisableLighting();

	camera.end();
	glDisable(GL_DEPTH_TEST);


}

//--------------------------------------------------------------
void ofApp::onCollision(ofxBulletCollisionData& cdata){
	cout << "collision" << endl;
	if (*player == cdata)
	{
		if (volume >= .1) volume -= .1;
		cout << volume << endl;
	}
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
