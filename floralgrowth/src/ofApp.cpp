#include "ofApp.h"

//--------------------------------------------------------------
bool isPrintInactive(ImagePrint& print) {
    return !print.isActive;
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofDirectory dir;
    int nFiles = dir.listDir("images");
    if(nFiles) {
        for(int i=0; i<dir.size(); i++) {
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().load(filePath);
        }
        
    }
    else ofLog(OF_LOG_WARNING) << "Could not find images folder";
    
    noise = 0.0;
    
    // GUI
    gui.setup();
    parametersVisual.setName("visual");
    parametersVisual.add(printing.set("printing", true));
    parametersVisual.add(spreadRange.set("spread", 100, 0, 200));
    parametersVisual.add(scale.set("scale", 1, 0, 1));
    parametersVisual.add(colourMin.set("colour min", ofColor(255)));
    parametersVisual.add(depthRotationMin.set("depth rotation min", ofVec2f(90), ofVec2f(0), ofVec2f(360)));
    parametersVisual.add(depthRotationMax.set("depth rotation max", ofVec2f(180), ofVec2f(0), ofVec2f(360)));
    
    gui.add(parametersVisual);
    parametersMisc.setName("misc");
    parametersMisc.add(saveSettings.set("save", false));
    parametersMisc.add(loadSettings.set("load", false));
    gui.add(parametersMisc);
    gui.loadFromFile("settings.xml");
    
    ofHideCursor();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofRemove(prints, isPrintInactive);
    for (auto & print: prints) {
        print.update();
    }
    
    if (printing) {
        //int tmpX = ofMap(ofNoise(noise), 0.0, 1.0, 0.0, ofGetWidth()) + ofRandom(-spreadRange, spreadRange);
        //int tmpY = ofMap(ofNoise(noise + 1000), 0.0, 1.0, 0.0, ofGetHeight()) + ofRandom(-spreadRange, spreadRange);
        int tmpX = ofGetMouseX() + ofRandom(-100, 100);
        int tmpY = ofGetMouseY() + ofRandom(-100, 100);
        RandomPrint(tmpX, tmpY);
    }
    noise += 0.1;

    
    // GUI
    if (saveSettings) {
        saveSettings = false;
        gui.saveToFile("settings.xml");
    }
    if (loadSettings) {
        loadSettings = false;
        gui.loadFromFile("settings.xml");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(10);
    //for (int imageIndex = 0; imageIndex < images.size(); imageIndex++) {
    //images[imageIndex].getTexture().bind();
    for (auto & print: prints) {
        //if (print.imageIndex == imageIndex)
        print.draw();
    }
    //images[imageIndex].getTexture().unbind();
    //}
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()) + "\n " + ofToString(prints.size()) + " prints", 20, ofGetHeight()-20);
    gui.draw();
}


//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------
void ofApp::RandomPrint(int x, int y) {
    int randomI = ofRandom(1, images.size());
    Print(randomI, x, y);
}

//--------------------------------------------------------------
void ofApp::Print(int imageIndex, int x, int y) {
    ImagePrint print;
    print.position.set(x, y);
    print.scaleTarget = ofRandom(0.15f, 0.4f) * scale;
    float targetRotation = ofRandom(0, 360);
    print.rotation.set(ofRandom(depthRotationMin.get().x, depthRotationMax.get().x),
                       ofRandom(depthRotationMin.get().y, depthRotationMax.get().y),
                       targetRotation + ofRandom(-60, 60));
    print.rotationTarget.set(0, 0, targetRotation);
    print.image = &images[imageIndex];
    print.imageIndex = imageIndex;
    print.colour.set(ofRandom(colourMin.get().r, 255),
                     ofRandom(colourMin.get().g, 255),
                     ofRandom(colourMin.get().b, 255));
    prints.push_back(print);
}


//////////////////////////////////////////////////////////////////////////////////
// GUI event handlers
//////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------
void saveSettingsChanged(bool & save){
    ofLogNotice() << "save";
}


//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
    if (key == 'd') {
        printing = !printing;
    }
    if (key == 'c') {
        colourMin.set(ofColor(ofRandom(100, 255), ofRandom(100, 255), ofRandom(100, 255)));
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//    if (printing) {
//        int tmpX = x + ofRandom(-spreadRange, spreadRange);
//        int tmpY = y + ofRandom(-spreadRange, spreadRange);
//        RandomPrint(tmpX, tmpY);
//    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (printing) {
        int n = 30;
        float angleDelta = (PI * 2) / n;
        float angle = 0;
        for (int i=0; i < n; i++) {
            float range = 200;
            int tmpX = x + (sin(angle)*range);
            int tmpY = y + (cos(angle)*range);
            RandomPrint(tmpX, tmpY);
            angle += angleDelta;
        }
        Print(0, x, y);
    }
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
