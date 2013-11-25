#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Integrator.h"
class testApp : public ofBaseApp{
    
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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void superShape();
    
	ofxOscReceiver oscIn;
	ofxOscSender oscOut;
    vector<float> fftData;
    float fftMax;
    
    Integrator iRadius;
    Integrator a;
    Integrator b;
    Integrator m;
    Integrator n1;
    Integrator n2;
    Integrator n3;
    
    ofMesh mesh;
    vector<ofVec2f> triangle;
    
    float pt1,pt2,pt3;
    float scale;
    ofPolyline line;
    ofPolyline* curLine;
    
    bool saveFrame;
    float radCurve;
};
