#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ImagePrint.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void RandomPrint(int x, int y);
    void Print(int imageIndex, int x, int y);
    
    float noise;
    
    vector <ofImage> images;
    vector <ImagePrint> prints;
    
    ofxPanel gui;
    ofParameterGroup parametersVisual;
    ofParameter<bool> printing;
    ofParameter<float> scale;
    ofParameter<float> spreadRange;
    ofParameter<ofColor> colourMin;
    ofParameter<ofVec2f> depthRotationMin;
    ofParameter<ofVec2f> depthRotationMax;
    
    ofParameterGroup parametersMisc;
    ofParameter<bool> saveSettings;
    ofParameter<bool> loadSettings;
    
    
    void saveSettingsChanged(bool & save);
    
    
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
};
