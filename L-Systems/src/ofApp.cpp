#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    current = "A";
    
    turtle.setPos(ofGetWidth()/2, ofGetHeight()/2);
    turtle.setDeltaA(22.7);
    
    ofBackground(0);
    ofSetLineWidth(2);
    drawn = false;
    count = -1;
    
    cam.lookAt(turtle.getPos());
    
    cam.setFarClip(500000);
    
    gui.setup();
    gui.add(x.set("X", ofGetWidth()/2, 0, ofGetWidth()));
    gui.add(y.set("Y", ofGetHeight()/2, 0, ofGetHeight()));
    gui.add(scale.set("Scale", 1.0, 0.001, 2.0));
    
    ofEnableDepthTest();
}

//--------------------------------------------------------------
string ofApp::apply(string input){
    stringstream output;
    for(auto it = input.begin(); it != input.end(); it++) {
        
        switch(*it) {
            case 'A' :
                output<<"X";
                break;
            case 'X' :
                output<<"-FF[&X][^^X]F[//-+X&FFX";
                break;
            case 'F' :
                output<<"FF";
                break;
            default:
                output<<*it;
                break;
        }
    }
    return output.str();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString(ofToString(count), 10, 20);
    
    cam.begin();
    ofPushMatrix();
    ofSetColor(127);
    ofDrawPlane(0, 0, 0, 100, 100);
//    ofTranslate(x, y);
    ofScale(scale);
    turtle.draw();
    ofPopMatrix();
    cam.end();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        count++;
        current = apply(current);
        turtle.clearLine();
        turtle.clearLine();
        turtle.setPos(0, 0);
        turtle.setDist(10 * scale);
        turtle.setOrientation(0, 0, 1, 1, 0, 0, 0, 1, 0);
        for(auto it = current.begin(); it != current.end(); it++) {
            turtle.step(*it);
        }
        //turtle.setDist(turtle.getDist()*1.5);
    }
    if(key == 'f') {
        ofToggleFullscreen();
    }
//    cout<<current<<endl;
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
