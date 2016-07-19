//
//  point.h
//  test_PatternReveal
//
//  Created by cafe on 18/07/2016.
//
//

#ifndef point_h
#define point_h

#include "ofMain.h"

class point {
public:
    point() {
        rotationOffset = ofRandom(0, 360);
    };
    
    //Setters
    void setPos(int _x, int _y) {
        x = _x;
        y = _y;
    }
    void setMaxRadius(float _rad) { maxRadius = _rad; }
    void setRadius(float _rad) { radius = _rad; };
    void setHitBox(float _rad) { hitBox = _rad; };
    void setImg(ofImage* _img) { img = _img; };
    void setColor(ofParameter<ofColor>* _col) { col = _col; };
    //Getters
    
    //Functionality
    void update(bool _active) {
        if(_active) {
            radius = ofLerp(radius, maxRadius, 0.05);
        } else {
            radius = ofLerp(radius, 0.0, 0.015);
        }
        rotationZ = rotationOffset + ofMap(radius, 0.0, maxRadius, -180, 180);
        rotationX = ofMap(radius, 0.0, maxRadius, rotationOffset, 0.0);
    }
    
    void update(int _x, int _y) {
        if(ofDist(_x, _y, x, y) < hitBox) {
            active = true;
        } else if(maxRadius - radius < 0.5) {
            active = false;
        }
        update(active);
    }
    
    void draw() {
        if(radius > 5) {
            ofPushStyle();
            ofPushMatrix();
            float alpha = ofMap(radius, 0.0, maxRadius, 127, 300);
            ofSetColor(col->get().r, col->get().g, col->get().b, alpha);
            ofTranslate(x, y);
            ofRotateZ(rotationZ);
            ofRotateX(rotationX);
            img->draw(0, 0, radius, radius);
            ofPopMatrix();
            ofPopStyle();
        }
    };
    
//private:
    float x, y;
    float maxRadius, radius, hitBox, rotationZ, rotationX, rotationOffset;
    bool active;
    ofImage* img;
    ofParameter<ofColor>* col;
};

#endif /* point_h */
