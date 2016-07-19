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
    
    box2d.init();
    box2d.setGravity(0, 2);
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
//    for(int x = 0; x < 25; x++) {
//        for(int y = 0; y < 25; y++) {
//            point* newPoint = new point();
//            newPoint->setRadius(0.0f);
//            newPoint->setMaxRadius(50.0f);
//            newPoint->setHitBox(70.0f);
//            newPoint->setPos(ofGetWidth()/2 + ofRandom(-100, 100), ofGetWidth() / 100 + y * ofGetHeight() / 50);
//            newPoint->setImg(images[(int)ofRandom(0.0, images.size())]);
//            newPoint->setColor(&minCol);
//            newPoint->setPhysics(0.4, 0.53, 0.31);
//            newPoint->setup(box2d.getWorld(), newPoint->x, newPoint->y, newPoint->radius);
//            points.push_back(newPoint);
//        }
//    }
    for(int i = 0; i < 2000; i++) {
        shared_ptr<point> newPoint = shared_ptr<point>(new point);
        newPoint->setRadius(10.0f);
        newPoint->setMaxRadius(50.0f);
        newPoint->setHitBox(70.0f);
        newPoint->setPos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        newPoint->setImg(images[(int)ofRandom(0.0, images.size())]);
        newPoint->setColor(&minCol);
        newPoint->setPhysics(0.4, 0.53, 0.31);
        newPoint->setup(box2d.getWorld(), newPoint->x, newPoint->y, newPoint->radius);
        points.push_back(newPoint);
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
    box2d.update();
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
//        points[i]->y += ofMap(ofNoise(noise + points[i]->x/50), 0.0f, 1.0f, 0.0f, 2.0f);
//        points[i]->x += ofMap(ofNoise(noise), 0.0f, 1.0f, 0.0f, 2.0f);

        if (points[i]->y > ofGetHeight()) {
            points[i]->setPosition(points[i]->x, ofRandom(-ofGetHeight(), 0));
            points[i]->setVelocity(0.0, ofRandom(0.0, 5.0));
        }
        
        if (points[i]->x > ofGetWidth()) {
            points[i]->setPosition(0, points[i]->y);
        } else if(points[i]->x < 0) {
            points[i]->setPosition(ofGetWidth(), points[i]->y);

        }
//        points[i]->setHitBox(ofMap(ofNoise(noise + 2000), 0.0f, 1.0f, 70.0f, 100.0f));
//        points[i]->update(ofMap(ofNoise(noise), 0.0, 1.0, 0.0, ofGetWidth()), ofMap(ofNoise(noise + 1000), 0.0, 1.0, 0.0, ofGetHeight()));
        //points[i]->update(x, y);
#endif
        
//        if(ofGetMousePressed()) {

//        }
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
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 100, ofGetHeight() - 100);
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
//    for(int x = 0; x < 25; x++) {
//        for(int y = 0; y < 25; y++) {
            shared_ptr<point> newPoint = shared_ptr<point>(new point);
            newPoint->setRadius(10.0f);
            newPoint->setMaxRadius(50.0f);
            newPoint->setHitBox(70.0f);
            newPoint->setPos(x, y);
            newPoint->setImg(images[(int)ofRandom(0.0, images.size())]);
            newPoint->setColor(&minCol);
            newPoint->setPhysics(0.4, 0.53, 0.31);
            newPoint->setup(box2d.getWorld(), newPoint->x, newPoint->y, newPoint->radius);
            points.push_back(newPoint);
//        }
//    }
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
