//
//  ImagePrint.cpp
//  imageSequenceExample
//
//  Created by Chris Mullany on 18/07/2016.
//
//

#include "ImagePrint.h"

ImagePrint::ImagePrint() {
    image = NULL;
    position.set(0);
    rotation.set(0);
    rotationTarget.set(0);
    scale = 0;
    scaleTarget = 0;
    age = 0;
    ageLimit = ofRandom(360, 900);
    rotationOffset.set(ofRandom(0, 1), ofRandom(0, 1));
    brightness = 255;
    isActive = true;
    imageIndex = 0;
    depth = ofRandom(0, 10);
    colour.set(255);
    textureOffset.set(200, 200);
    randomAngle = ofRandom(0, PI*2);
}

void ImagePrint::setup() {
}

void ImagePrint::update() {
    if (image != NULL) {
        if (age++ < ageLimit) {
            
            // animate rotation, texture offset and scale
            rotation.x = ofLerp(rotation.x, rotationTarget.x, 0.1f);
            rotation.y = ofLerp(rotation.y, rotationTarget.y, 0.1f);
            rotation.z = ofLerp(rotation.z, rotationTarget.z, 0.1f);
            textureOffset.x = ofLerp(textureOffset.x, 0, 0.1f);
            textureOffset.y = ofLerp(textureOffset.y, 0, 0.1f);
            scale = ofLerp(scale, scaleTarget, 0.1f);
            
            // update the rectangle for drawing the image vertices
            // position is centered but with an offset applied for randomness
            vertRect.set(-image->getWidth() * rotationOffset.x, -image->getHeight() * rotationOffset.y, image->getWidth(), image->getHeight());
            
            // update the texture coordinates rectangle
            textRect.set(textureOffset.x, textureOffset.y, image->getWidth(), image->getHeight());
        }
        else {
            // close / shrink / fade
            brightness = ofLerp(brightness, 0, 0.02f);
            scale = ofLerp(scale, 0, 0.01f);
            if (brightness < 2) {
                isActive = false;
            }
        }
    }
}

void ImagePrint::draw() {
    if (image != NULL) {
        
        // tint and apply alpha
        ofSetColor(colour, brightness);
        
        ofPushMatrix();
        {
            ofTranslate(position.x, position.y, depth);
            ofRotateZ(rotation.z);
            ofRotateX(rotation.x);
            ofRotateY(rotation.y);
            ofScale(scale, scale);
            
            image->getTexture().bind();
            {
                DrawQuad();
            }
            image->getTexture().unbind();
        }
        ofPopMatrix();
    }
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////


void ImagePrint::DrawQuad(){
    glBegin(GL_QUADS);
    glTexCoord2f(textRect.getTop(), textRect.getLeft());
    glVertex3f(vertRect.getTop(), vertRect.getLeft(), 0);
    glTexCoord2f(textRect.getTop(), textRect.getRight());
    glVertex3f(vertRect.getTop(), vertRect.getRight(), 0);
    glTexCoord2f(textRect.getBottom(), textRect.getRight());
    glVertex3f(vertRect.getBottom(), vertRect.getRight(), 0);
    glTexCoord2f(textRect.getBottom(), textRect.getLeft());
    glVertex3f(vertRect.getBottom(), vertRect.getLeft(), 0);
    glEnd();

}

void ImagePrint::DrawTriangles(){
    int n = 8;
    float angleDelta = (PI * 2) / n;
    float angle = 0;
    float range = textRect.width * 0.8;
    ofPoint centre = textRect.getCenter();
    ofPoint textP1 = centre + ofPoint(sin(randomAngle)*range, cos(randomAngle)*range);
    ofPoint textP2 = centre + ofPoint(sin(randomAngle + angleDelta)*range, cos(randomAngle + angleDelta)*range);
    
    glBegin(GL_TRIANGLES);
    
    for (int i=0; i < n; i++) {
        
        int tmpX = centre.x + (sin(angle)*range);
        int tmpY = centre.y + (cos(angle)*range);
        glTexCoord2f(textP1.x, textP1.y);
        glVertex3f(tmpX, tmpY, 0);
        
        angle += angleDelta;
        tmpX = centre.x + (sin(angle)*range);
        tmpY = centre.y + (cos(angle)*range);
        glTexCoord2f(textP2.x, textP2.y);
        glVertex3f(tmpX, tmpY, 0);
        
        glTexCoord2f(centre.x, centre.y);
        glVertex3f(centre.x, centre.y, 0);
        
    }
    
    glEnd();
}

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////
