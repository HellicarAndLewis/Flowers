#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(50);
	ofSetFrameRate(30);

	fft.setMirrorData(false);
	fft.setup();

	string guiPath = "audio.xml";
	gui.setup("audio", guiPath, 20, 20);
	gui.add(audioThreshold.setup("audioThreshold", 1.0, 0.0, 1.0));
	gui.add(audioPeakDecay.setup("audioPeakDecay", 0.915, 0.0, 1.0));
	gui.add(audioMaxDecay.setup("audioMaxDecay", 0.995, 0.0, 1.0));
	gui.add(audioMirror.setup("audioMirror", false));
	gui.loadFromFile(guiPath);

	fft.setThreshold(audioThreshold);
	fft.setPeakDecay(audioPeakDecay);
	fft.setMaxDecay(audioMaxDecay);
	fft.setMirrorData(audioMirror);

	meshOriginal = meshWarped = ofMesh::sphere(200, 30);

#ifndef LIVE_INPUT
	ofxNestedFileLoader loader;
	vector<string> audioFiles = loader.load("tracks");
	if (audioFiles.size() > 0)
		player.load(audioFiles[0]);
	else
		ofLogError("No audio files loaded because data/audio folder is empty!");
	player.setLoop(true);
	player.play();
	player.setVolume(1.0);
#endif

	sender.setup(HOST, PORT);
}

//--------------------------------------------------------------
void ofApp::update() {

	fft.setThreshold(audioThreshold);
	fft.setPeakDecay(audioPeakDecay);
	fft.setMaxDecay(audioMaxDecay);
	fft.setMirrorData(audioMirror);
	fft.update();

	//---------------------------------------------------------- dispacing mesh using audio.
	vector<ofVec3f> & vertsOriginal = meshOriginal.getVertices();
	vector<ofVec3f> & vertsWarped = meshWarped.getVertices();
	int numOfVerts = meshOriginal.getNumVertices();

	float * audioData = new float[numOfVerts];
	fft.getFftPeakData(audioData, numOfVerts);
	data = fft.getFftPeakData();

	float meshDisplacement = 100;

	ofxOscMessage message;

	for (int i = 0; i<numOfVerts; i++) {
		float audioValue = audioData[i];
		ofVec3f & vertOriginal = vertsOriginal[i];
		ofVec3f & vertWarped = vertsWarped[i];

		ofVec3f direction = vertOriginal.getNormalized();
		vertWarped = vertOriginal + direction * meshDisplacement * audioValue;


	}
	message.setAddress("/Flowers");

	for (int i = 0; i < data.size(); i++) {
		message.addFloatArg(data[i]);
	}

	sender.sendMessage(message, false);

	delete[] audioData;
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);

	int w = OFX_FFT_WIDTH;
	int h = OFX_FFT_HEIGHT;
	int x = 20;
	int y = ofGetHeight() - h - 20;
	fft.draw(x, y, w, h);

	ofSetColor(255);

	cam.begin();
	meshWarped.drawWireframe();
	cam.end();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}