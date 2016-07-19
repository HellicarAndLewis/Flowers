#pragma once

#include "ofMain.h"
#include "point.h"
#include "ofxNestedFileLoader.h"
#include "ofxCv.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
    
        ofVideoGrabber camera;
    
        ofxCv::FlowFarneback fb;
    
        ofxCv::Flow* curFlow;
    
        ofImage pattern;
    
        vector< shared_ptr<point> > points;
        vector<ofImage*> images;
    
        ofxBox2d                                box2d;           // the box2d world
    
        float noise;
    
        bool showGui;
        bool showCursor;
        ofxPanel gui;
        ofParameter<ofColor> minCol;
    
        ofFbo buffer;
};
