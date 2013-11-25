#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    //    ofSetVerticalSync(true);
	ofSetFrameRate(120);
	ofHideCursor();
	oscIn.setup(12345);
	oscOut.setup("surya.local", 12346);
    fftMax =0;
    scale =100;
    ofEnableSmoothing();
    for(int i=0;i<10;i++){
        line.addVertex(100,0);
        line.addVertex(-100,0);
        line.addVertex(0,-100);
    }
    ofEnableSmoothing();
    saveFrame =false;
    
    iRadius = Integrator(200,.2f,.2f);
    a = Integrator(1,.2f,.2f);
    b = Integrator(1,.2f,.2f);
    m =  Integrator(2,.2f,.2f);
    n1 = Integrator(20,.2f,.2f);
    n2 = Integrator(1,.2f,.2f);
    n3 = Integrator(1,.2f,.2f);
    
    stkWeight = Integrator(22,.2f,.2f);
    
    iRadius_ =200;
    a_ = 1;
    b_ = 1;
    m_ = 2;
    n1_ =20;
    n2_ = 1;
    n3_ = 1;
    stkWeight_ = 22;


    setupGUI();
}
void testApp::setupGUI(){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
//    gui->setFont("GUI/")
    
    gui = new ofxUISuperCanvas("SUPER SHAPER");
    gui->addSpacer();
    gui->addSlider("RADIUS", 0, 500, &iRadius_);  //Parameters("name", low, high, default_value, x, y, length, height)
    gui->addSlider("A", 0.1, 5.0, &a_);
    gui->addSlider("B", 0.1, 5, &b_);
    gui->addSlider("M", 0, 20, &m_);
    gui->addSlider("N1", 0, 100, &n1_);
    gui->addSlider("N2", -50, 100, &n2_);
    gui->addSlider("N3", -50, 100, &n3_);
    gui->addSlider("STROKE", -50, 100, &stkWeight_);
    gui->addSpacer();
    gui->autoSizeToFitWidgets();

    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
}

void testApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name == "RADIUS") {
        iRadius.setTarget(iRadius_);
    }
    if (name == "A") {
        a.setTarget(a_);
    }
    if (name == "B") {
        b.setTarget(b_);
    }
    if (name == "M") {
        m.setTarget(m_);
    }
    if (name == "N1") {
        n1.setTarget(n1_);
    }
    if (name == "N2") {
        n2.setTarget(n2_);
    }
    if (name == "N3") {
        n3.setTarget(n3_);
    }
    if (name == "STROKE") {
        stkWeight.setTarget(stkWeight_);
    }
}
//--------------------------------------------------------------
void testApp::update(){
	ofxOscMessage msgIn;
	while(oscIn.getNextMessage(&msgIn)) {
        cout<<msgIn.getAddress()<<endl;
		if(msgIn.getAddress() == "/audio") {
            fftData.clear();
            
            
            fftMax =0;
            for(int i =0; i< msgIn.getNumArgs(); i++ ){
                fftData.push_back(msgIn.getArgAsFloat(i));
                fftMax = MAX(fftMax, fftData[i]);
                
            }
            //2.7 and 1.1
            float mapB = ofMap(fftData[fftData.size()/2],0,fftMax, 0.1,5.);
            b_ = mapB;
            b.setTarget(mapB);
            cout<<fftMax<<endl;
            
		}
		if(msgIn.getAddress() == "/saveFrame") {
			string filename = msgIn.getArgAsString(0);
            saveFrame = true;
            
		}
		if(msgIn.getAddress() == "/Switch") {
		}
		if(msgIn.getAddress() == "/radCurve") {
            radCurve    = msgIn.getArgAsFloat(0);
            
		}
		if(msgIn.getAddress() == "/duration/info") {
			string info = msgIn.getArgAsString(0);
            cout<<info<<endl;
            //			ofDirectory::createDirectory(directory, true, true);
		}
	}
    
    iRadius.update();
    a.update();
    b.update();
    m.update();
    n1.update();
    n2.update();
    n3.update();
    stkWeight.update();

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(128, 0);
    ofBackground(ofColor::black);
    //if(fftData.size()>0){
    //    line.resize(fftData.size());
     //   int rectWidth = ofGetWidth()/fftData.size();
        ofSetColor(ofColor::white);
        
//        ofSetLineWidth(2);
        ofNoFill();
        
        ofSetColor(ofColor::white);
        
        glPushMatrix();

//        glTranslatef(ofGetWidth()/2, ofGetHeight()/2, 0);
/*
        ofBeginShape();
        for (int i = 0; i <fftData.size(); i++){
            float val =fftMax - fftData[i] * 128;
//            ofRect(i*rectWidth, 0, rectWidth,val);
//            ofCircle(ofGetWidth()/2, -ofGetHeight()/2, val*100);
            
        }
        
        float x,y;
        float radius = 100;
        float centX = ofGetWidth()/2;
        float centY = -ofGetHeight()/2      ;
        
        for(int i =0; i< 360; i++){
            int index = ofMap(i, 0, 360, 0, fftData.size());
            
            float radVariance =ofMap(fftData[index], 0, fftMax, 0, 1);
            float thisRadius = radCurve + radVariance*100;
            float rad = ofDegToRad(i);
            x = centX + (thisRadius * cos(rad));
            y = centY + (thisRadius * sin(rad));
            ofCurveVertex(x, y);
            

        }
 
        ofEndShape();
    */
        ofSetLineWidth(stkWeight.value);
        superShape();
        glPopMatrix();
        
    //}
    if(saveFrame){
        ofSaveFrame();
        cout<<"savingFrame!"<<endl;
        saveFrame = false;
    }

/*
 glPushMatrix();
 glTranslatef(ofGetWidth()/2, ofGetHeight()/2, 0);
 //    line.draw();
 //    ofBeginShape();
 //    ofVertex(0, fftMax * scale);
 //    ofVertex(-fftMax * scale, -fftMax*scale);
 //    ofVertex(fftMax * scale, -fftMax * scale);
 //    ofEndShape();
 glPopMatrix();
 
 */
}

void testApp::superShape(){
    
    ofBeginShape();
    for(float theta = 0; theta <TWO_PI+0.001f; theta+=0.003f)
    {
        float raux  = pow(abs(1.0f/a.value)*abs(cos((m.value*theta/4.0f))),n2.value) + pow(abs(1.0f/b.value)*abs(sin(m.value*theta/4.0f)),n3.value);
        float r = iRadius.value*pow(abs(raux),(-1.0f/n1.value));
        float x=ofGetWidth()*.5f+r*cos(theta);
        float y=ofGetHeight()*.5f+r*sin(theta);
        ofVertex(x,y);
    }
    
    ofEndShape(true);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key=='f'){
        ofToggleFullscreen();
    }
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
