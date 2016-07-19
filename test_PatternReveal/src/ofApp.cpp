#include "ofApp.h"
//#define USING_CAMERA

//--------------------------------------------------------------
void ofApp::setup(){
    pattern.load("images/FLORALSmallScale.png");
    
    ofxNestedFileLoader loader;
    vector<string> flowerNames = loader.load("images/flowers");
    for(int i = 0; i < flowerNames.size(); i++) {
        ofImage* newImage = new ofImage();
        newImage->load(flowerNames[i]);
        images.push_back(newImage);
    }
    
    for(int x = 0; x < 50; x++) {
        for(int y = 0; y < 50; y++) {
            point* newPoint = new point();
            newPoint->setRadius(0.0f);
            newPoint->setMaxRadius(50.0f);
            newPoint->setHitBox(70.0f);
            newPoint->setPos(ofGetWidth() / 100 + x * ofGetWidth() / 50, ofGetWidth() / 100 + y * ofGetHeight() / 50);
            newPoint->setImg(images[(int)ofRandom(0.0, images.size())]);
            newPoint->setColor(&minCol);
            points.push_back(newPoint);
        }
    }
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofBackground(0);
    
    //buffer.allocate(1280, 720);
    
    gui.setup();
    gui.add(minCol.set("minCol", ofColor(255)));
    
    ofHideCursor();
    
    showGui = true;
    showCursor = false;
    
    noise = 0.0;
    
#ifdef USING_CAMERA
    camera.setup(320, 240);
    
    fb.setPyramidScale(0.5f);
    fb.setNumLevels(4);
    fb.setWindowSize(32);
    fb.setNumIterations(2);
    fb.setPolyN(7);
    fb.setPolySigma(1.5);
    fb.setUseGaussian(false);
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
#ifdef USING_CAMERA
    camera.update();
    
    if(camera.isFrameNew()) {
        fb.calcOpticalFlow(camera);
    }
#else
    int x = ofGetMouseX();
    int y = ofGetMouseY();
#endif
    for(int i = 0; i < points.size(); i++) {
#ifdef USING_CAMERA
        float flowX = ofMap(points[i]->x, 0, ofGetWidth(), 1, 320 - 1, true);
        float flowY = ofMap(points[i]->y, 0, ofGetHeight(), 1, 240 - 1, true);
        ofVec2f flow = fb.getAverageFlowInRegion(ofRectangle(flowX, flowY, 1, 1));
        if(flow.length() > 1.0) {
            points[i]->update(true);
        } else {
            points[i]->update(false);
        }
#else
        points[i]->update(true);
        points[i]->y += ofMap(ofNoise(noise + points[i]->x/50), 0.0f, 1.0f, 0.0f, 2.0f);
        points[i]->x += ofMap(ofNoise(noise), 0.0f, 1.0f, 0.0f, 2.0f);

        if (points[i]->y > ofGetHeight()) {
            points[i]->y = 0;
        } if (points[i]->x > ofGetWidth()) {
            points[i]->x = 0;
        }
//        points[i]->setHitBox(ofMap(ofNoise(noise + 2000), 0.0f, 1.0f, 70.0f, 100.0f));
//        points[i]->update(ofMap(ofNoise(noise), 0.0, 1.0, 0.0, ofGetWidth()), ofMap(ofNoise(noise + 1000), 0.0, 1.0, 0.0, ofGetHeight()));
        //points[i]->update(x, y);
#endif
    }
    noise += 0.01;
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef USING_CAMERA
    ofTranslate(ofGetWidth(), 0);
    ofScale(-1, 1);
#endif
    for(int i = 0; i < points.size(); i++) {
        points[i]->draw();
    }
    if(showGui) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'f' :
            ofToggleFullscreen();
            break;
        case 'g' :
            showGui = !showGui;
            break;
        case 'm' :
            if(showCursor) {
                ofHideCursor();
            } else {
                ofShowCursor();
            }
            showCursor = !showCursor;
            break;
        default :
            break;
    }
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
