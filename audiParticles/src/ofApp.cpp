#include "ofApp.h"

#define WIDTH ofGetWidth()
#define HEIGHT ofGetHeight()

//-----------------------------------------------------------------------------------------
//
void ofApp::setup()
{
	//ofSetLogLevel( OF_LOG_VERBOSE );
    
	ofSetFrameRate( 60 );
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	
	ofxGuiSetDefaultWidth( 300 );

	int texSize = 128 * 2;
	particles.init( texSize ); // number of particles is (texSize * texSize)
	
	// Give us a starting point for the camera
	camera.setNearClip( 0.01f );
	camera.setPosition( 0, 0.4, 0.5 );
	camera.setMovementMaxSpeed( 0.01f );

	time = 0.0f;

	drawGui = false;
    
    drawCam = false;
    
    //Setup the videoGrabber
    grabber = new ofVideoGrabber();
    
    grabber->initGrabber( 160, 120 );
    
    //Setup the kinect
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    depthFinder.setup(kinect.getWidth(), kinect.getHeight());
    
    //setup our FBO
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    elementsDir.open("Settings/Elements");
    elementsDir.listDir();
    elements.resize(elementsDir.numFiles());
    for(int i = 0; i < elementsDir.numFiles(); i++) {
        string path = elementsDir.getPath(i);
        elements[i].loadFromFile(path);
    }
    
    //Setup the gui for controlling input and output
    string xmlSettingsPath = "Settings/InputOutput.xml";
    gui.setup("input/output", xmlSettingsPath);
    gui.setPosition(ofGetWidth()/2, 10);
    gui.add(input.set("Input", KINECT, 0, 1));
    gui.add(output.set("Element", 0, 0, elementsDir.numFiles()-1));
    gui.add(flowControl.set("Flow Interaction", true));
    gui.add(kinectSpawn.set("Point Cloud Spawn", false));
    gui.add(nearClip.set("Kinect Spawn Near Clip", 230, 0, 255));
    gui.add(farClip.set("Kinect Spawn Far Clip", 70, 0, 255));
    gui.add(vignetteOffset.set("Vignette Offset", 0.0, 0.0, 500.0));
    gui.add(depthReactivity.set("Depth Reaction", false));
    gui.loadFromFile(xmlSettingsPath);
    
//    elements.resize(4);
//    elements[0].loadFromFile("Settings/Elements/Fire.xml");
//    elements[1].loadFromFile("Settings/Elements/Water.xml");
//    elements[2].loadFromFile("Settings/Elements/Earth.xml");
//    elements[3].loadFromFile("Settings/Elements/Air.xml");
    
    lastOutput = elementsDir.numFiles();
    
    ofSetWindowShape(1080, 1920);
    ofSetWindowPosition(ofGetScreenWidth(), 0);
//    fbo.allocate(WIDTH, HEIGHT);
    
    vignette.loadImage("Textures/Vignette_white_001.png");
    
    noise = 0.0;
    
}


//-----------------------------------------------------------------------------------------
//
void ofApp::update()
{
//     agua.update(&fbo.getTextureReference());
    if(output != lastOutput) {
        elements[output].setToParticleSystem(&particles, &camera, &kinectSpawn, &vignetteOffset, &depthReactivity);
        lastOutput = output;
    }
	// Update time, this let's us hit space and slow down time, even reverse it.
//	if( ofGetKeyPressed(' ') ) { timeStep = ofLerp( timeStep, ofMap( ofGetMouseX(), 0, ofGetWidth(), -(1.0f/60.0f), (1.0f/60.0f) ), 0.1f );}
//	else { timeStep = ofLerp( timeStep, 1.0f / 60.0f, 0.1f ); }
	time += particles.timeStep;
    
    if(input == KINECT) {
        kinect.update();
        if(kinect.isFrameNew()) {
            img.setFromPixels(kinect.getPixels(), kinect.getWidth(), kinect.getHeight(), OF_IMAGE_COLOR);
            img.setImageType(OF_IMAGE_GRAYSCALE);
            img.resize(grabber->getWidth(), grabber->getHeight());
            flowFinder.calcOpticalFlow(img);
            ofVec2f flow = flowFinder.getAverageFlow();
            if(flowControl)
                particles.modifyByVector(flow);
            if(depthReactivity) {
                depthFinder.updateAverageDepth(&kinect);
                float change = depthFinder.getCurrentAverageDepth();
                change = ofMap(change, 0.5, 1, 0.001, 0.01, true);
                particles.modifyTimeStep(change);
            }
        }
    }
    if(input == CAMERA) {
        grabber->update();
        if(grabber->isFrameNew()) {
            img.setFromPixels(grabber->getPixels(), grabber->getWidth(), grabber->getHeight(), OF_IMAGE_COLOR);
            img.setImageType(OF_IMAGE_GRAYSCALE);
            flowFinder.calcOpticalFlow(img);
            ofVec2f flow = flowFinder.getAverageFlow();
            if(flowControl) {
                particles.modifyByVector(flow);
            }
            
        }
    }
    noise += 0.001;
}   




//-----------------------------------------------------------------------------------------
//
void ofApp::draw()
{
	ofBackgroundGradient( ofColor(40,40,40), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);
    if(elements[output].backgroundLoaded) {
        ofPushStyle();
        ofSetColor(particles.backgroundColor);
        elements[output].background.draw(0, 0, WIDTH, HEIGHT);
        ofPopStyle();
    } else {
        ofBackground(particles.backgroundColor);
    }
    setSpawnPointsContours();
    particles.update( time , spawnPoints);

	camera.begin();
	
		// draw a grid on the floor
		ofSetColor( ofColor(60) );
		ofPushMatrix();
			ofRotate(90, 0, 0, -1);
			//ofDrawGridPlane( 0.5, 12, false );
		ofPopMatrix();
		 
		ofSetColor( ofColor::white );
	
		particles.draw( &camera );
    
	camera.end();
    
	ofDisableDepthTest();
	ofEnableBlendMode( OF_BLENDMODE_ALPHA );
	ofSetColor( ofColor::white );
    
   // kinect->draw(420, 10, 400, 300);
	
//	int size = 196;
//	particles.particleDataFbo.source()->getTextureReference(0).draw( 0,	 0, size, size );
    if(elements[output].foregroundLoaded) {
        ofPushStyle();
        ofSetColor(particles.foregroundColor);
        elements[output].foreground.draw(0, 0, WIDTH, HEIGHT);
        ofPopStyle();
    }
    ofPushStyle();
    ofSetColor(ofColor::white/*elements[output].backgroundColor*/);
    vignette.draw(-vignetteOffset, -vignetteOffset, ofGetWidth() + vignetteOffset*2, ofGetHeight() + vignetteOffset*2);
    ofPopStyle();

	if( drawGui )
	{
		particles.gui.draw();
        gui.draw();
        fontSmall.drawStringShadowed(elements[output].name, ofGetWidth()/2, 10);
        //switch for which input we're using
        switch (input) {
            case KINECT:
                ofPushMatrix();
                ofTranslate(0, HEIGHT - kinect.getHeight());
                ofTranslate(kinect.getWidth(), 0);
                //ofScale(-1, 1);
//                contourFinder.draw();
//                ofScale(-1, 1);
//                kinect.draw(-WIDTH/2, HEIGHT - kinect.getHeight() - 10, kinect.getHeight(), kinect.getWidth());
//                kinect.drawDepth(-WIDTH/2 - kinect.getWidth(), HEIGHT - kinect.getHeight() - 10, kinect.getHeight(), kinect.getWidth());
//                contourFinder.draw(-WIDTH/2 - kinect.getWidth(), HEIGHT - kinect.getHeight() - 10, kinect.getHeight(), kinect.getWidth());
                ofPopMatrix();
                break;
            case CAMERA:
                ofScale(-1, 1);
                grabber->draw(-WIDTH/2, HEIGHT - grabber->getHeight() - 10, grabber->getWidth(), grabber->getHeight());
            default:
                break;
        }
	}
    if( drawCam ) {
        ofPushMatrix();
        ofTranslate(kinect.getWidth(), HEIGHT - kinect.getHeight());
        ofScale(-1, 1);
        kinect.draw(0, 0, kinect.getWidth(), kinect.getHeight());
        if(depthReactivity)
            depthFinder.debugDraw(0, 0, kinect.getWidth(), kinect.getHeight());
//        grayImage.draw(0, 0, kinect.getWidth() - 100, kinect.getHeight());
        if( kinectSpawn ) grayImage.draw(-kinect.getWidth(), 0, kinect.getWidth(), kinect.getHeight());
        ofPopMatrix();
    }
	
	ofDisableDepthTest();
	fontSmall.drawStringShadowed(ofToString(ofGetFrameRate(),2), ofGetWidth()-35, ofGetHeight() - 6, ofColor::whiteSmoke, ofColor::black );
}

//-----------------------------------------------------------------------------------------
//
void ofApp::keyPressed(int key)
{
	if( key == OF_KEY_TAB )
	{
		drawGui = !drawGui;
	}
    if( key == 'c' )
    {
        drawCam = !drawCam;
    }
	else if( key == 'f' )
	{
		ofToggleFullscreen();
	}
	else if( key == OF_KEY_LEFT  )
	{
        output--;
        if(output <= 0) output = 0;
	}
	else if( key == OF_KEY_RIGHT  )
	{
        output++;
        if(output >= elementsDir.numFiles()) output = elementsDir.numFiles() - 1;
	}
    else if( key == ' ' )
    {
        elements[output].setFromCurrentSystem(&particles, &camera, &kinectSpawn, &vignetteOffset, &depthReactivity);
        elements[output].saveToFile(elementsDir.getPath(output));
    }
    
    switch (key) {
        case '1':
            output = 0;
            break;
        case '2':
            output = 1;
            break;
        case '3':
            output = 2;
            break;
        case '4':
            output = 3;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
//
void ofApp::setSpawnPointsPointCloud() {
    if(kinectSpawn) {
        int w = 640;
        int h = 480;
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_POINTS);
        int step = 20;
        spawnPoints.clear();
        map<float, ofVec3f> distanceKeyedWorldCoords;
        vector<float> distances;
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w; x += step) {
                if(kinect.getDistanceAt(x, y) > nearClip && kinect.getDistanceAt(x, y) < farClip) {
                    distances.push_back(kinect.getDistanceAt(x, y));
                    distanceKeyedWorldCoords[kinect.getDistanceAt(x, y)] = kinect.getWorldCoordinateAt(x, y);
    //                if(kinect.getDistanceAt(x, y) < 1500) {
    //                    mesh.addColor(kinect.getColorAt(x,y));
    //                    mesh.addVertex(kinect.getWorldCoordinateAt(x, y));

    ////                ofVec3f vec = ofVec3f(scaledX)
    //                    spawnPoints.push_back(ofVec3f(scaledX, scaledY, scaledZ));
    //                }
                }
            }
        }
        cout<<distances.size()<<endl;
        nth_element(distances.begin(), distances.begin() + 500, distances.end());
        for(int i = 0; i < distances.size() && i < 500; i++) {
            float scaledX = -1 * ofMap(distanceKeyedWorldCoords[distances[i]].x, -500, 500, -0.1, 0.1, true);
            float scaledY = ofMap(distanceKeyedWorldCoords[distances[i]].y, -500, 500, -0.1, 0.1, true);
            float scaledZ = ofMap(distanceKeyedWorldCoords[distances[i]].z, -500, 500, -0.1, 0.1, true);
            spawnPoints.push_back(ofVec3f(scaledX, scaledY, scaledZ));
        }
    }
    
//    while (spawnPoints.size() < 200) {
//        spawnPoints.push_back(0.1 * ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
//    }
}

void ofApp::setSpawnPointsContours() {
    spawnPoints.clear();
    
    while (spawnPoints.size() < 500) {
        float gate = spawnPoints.size() % 10;
        float width = 0.01;
        ofVec3f point = 0.01 * ofVec3f(ofRandom(-width, width), ofRandom(-width, width), ofRandom(-width, width));
        point.x += 0.04 * gate + 0.4 * ofNoise(noise + gate* 100);
//        if(spawnPoints.size()%2 == 0) {
//            point.x += flowFinder.getAverageFlow().x * 0.5;
//        }
        spawnPoints.push_back(point);
    }
}

float ofApp::getDepthChange() {
    depthFinder.updateAverageDepth(&kinect);
    float currentAverageDepth = depthFinder.getCurrentAverageDepth();
    float runningAverageDepth = depthFinder.getRunningAverageDepth();
    
    return currentAverageDepth - runningAverageDepth;
}
