//
//  Turtle.h
//  L-Systems
//
//  Created by cafe on 14/07/2016.
//
//

#ifndef Turtle_h
#define Turtle_h

#include "ofMain.h"

class Turtle {
public:
    struct state {
        ofVec3f pos;
        float angle;
    };
    Turtle() {  pos = ofVec3f(0, 0, 0);
                d = 100;
                a = 0;
                H = ofVec3f(1, 0, 0);
                L = ofVec3f(0, 1, 0);
                U = ofVec3f(0, 0, 1);
                orientation = ofMatrix3x3(H.x, H.y, H.z, L.x, L.y, L.z, U.x, U.y, U.z);
                deltaA = ofDegToRad(90);
                line.setMode(OF_PRIMITIVE_LINES);
    };
    //Setters
    void setPos(float _x, float _y) {   pos = ofVec3f(_x, _y, 0); };
    void setOrientation(float _a, float _b, float _c, float _d, float _e, float _f, float _g, float _h, float _i) {
        orientation = ofMatrix3x3(_a, _b, _c, _d, _e, _f, _g, _h, _i);
    };
    void setDist(float _d) { d = _d; };
    void setDeltaA(float _delta) { deltaA = ofDegToRad(_delta); };
    //Getters
    ofVec2f getPos() { return pos; };
    float getAngle() { return a; };
    float getDist() { return d; };
    float getDeltaA() { return deltaA; };
    //functionality
    void step(char input) {
        float newX;
        float newY;
        switch(input) {
            case 'F' :
                line.addVertex(pos);
                line.addColor(255);
                pos += d * ofVec3f(orientation.a, orientation.b, orientation.c);
//                newX = pos.x + d * cos(a);
//                newY = pos.y + d * sin(a);
//                pos = ofVec2f(newX, newY);
                line.addVertex(pos);
                line.addColor(255);
                break;
            case 'f' :
                pos += d * ofVec3f(orientation.a, orientation.b, orientation.c);
//                newX = pos.x + d * cos(a);
//                newY = pos.y + d * sin(a);
//                pos = ofVec2f(newX, newY);
                break;
            case '+' :
                //a += deltaA;
                orientation *= Ru(deltaA);
                break;
            case '-' :
                //a -= deltaA;
                orientation *= Ru(-deltaA);
                break;
            case '&' :
                //a -= deltaA;
                orientation *= Rl(deltaA);
                break;
            case '^' :
                //a -= deltaA;
                orientation *= Rl(-deltaA);
                break;
            case '\\' :
                //a -= deltaA;
                orientation *= Rh(deltaA);
                break;
            case '/' :
                //a -= deltaA;
                orientation *= Rh(-deltaA);
                break;
            case '|' :
                //a -= deltaA;
                orientation *= Ru(PI);
                break;
            case '[' :
                pushState();
                break;
            case ']' :
                popState();
                break;
        }
    };
    void draw() {
        line.draw();
        ofSetColor(255, 0, 0);
//        ofDrawLine(pos.x, pos.y, pos.z, 10*orientation.a, 10*orientation.b, 10*orientation.c);
//        ofSetColor(0, 255, 0);
//        ofDrawLine(pos.x, pos.y, pos.z, 10*orientation.d, 10*orientation.e, 10*orientation.f);
//        ofSetColor(0, 0, 255);
//        ofDrawLine(pos.x, pos.y, pos.z, 10*orientation.g, 10*orientation.h, 10*orientation.i);
    }
    void clearLine() {
        line.clear();
        pos = ofVec2f(0, ofGetHeight());
    }
    void pushState() {
        state currentState;
        currentState.pos = pos;
        currentState.angle = a;
        states.push_back(currentState);
    }
    void popState() {
        state newState = states[states.size()-1];
        pos = newState.pos;
        a = newState.angle;
        states.pop_back();
    }
    
private:
    ofMesh line;
    ofVec3f pos;
    ofVec3f H, L, U;
    ofMatrix3x3 orientation;
    ofMatrix3x3 Ru(float a) {
        return ofMatrix3x3(cos(a), sin(a), 0, -sin(a), cos(a), 0, 0, 0, 1);
    }
    ofMatrix3x3 Rl(float a) {
        return ofMatrix3x3(cos(a), 0, -sin(a), 0, 1, 0, sin(a), 0, cos(a));
    }
    ofMatrix3x3 Rh(float a) {
        return ofMatrix3x3(1, 0, 0, 0, cos(a), -sin(a), 0, sin(a), cos(a));
    }
    vector<state> states;
    float a;
    float deltaA;
    float d;
};

#endif /* Turtle_hpp */
