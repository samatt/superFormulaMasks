#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Integrator.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{
    
typedef enum{
    circle =0,
    supershape,
    supershapeFFT,
}drawMode;
    
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
    void circleFFT();
    void superShapeFFT();
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
    void setupShaper();
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
    Integrator stkWeight;

    float iRadius_;
    float a_;
    float b_;
    float m_;
    float n1_;
    float n2_;
    float n3_;
    float stkWeight_;
    
    bool bFFT;
    ofMesh mesh;
    drawMode currentMode;
    vector<ofVec2f> temp, saved;
    float pt1,pt2,pt3;
    float scale;
    ofPolyline line;
    ofPolyline* curLine;
    
    bool saveFrame;
    float radCurve;
    
    ofxUISuperCanvas * gui;
};
