//
//  ImagePrint.h
//  imageSequenceExample
//
//  Created by Chris Mullany on 18/07/2016.
//
//

#pragma once
#include "ofMain.h"

class ImagePrint {
public:
    ImagePrint();
    
    void setup();
    void update();
    void draw();
    
    void DrawQuad();
    void DrawTriangles();
    
    float age;
    float ageLimit;
    bool isActive;
    
    ofImage* image;
    int imageIndex;
    ofRectangle vertRect;
    ofRectangle textRect;
    
    ofVec3f position;
    ofVec3f rotationOffset;
    ofVec3f rotation;
    ofVec3f rotationTarget;
    ofVec2f textureOffset;
    
    float scale;
    float scaleTarget;
    float brightness;
    float depth;
    float randomAngle;
    
    ofColor colour;
    
    
protected:
private:
    
};