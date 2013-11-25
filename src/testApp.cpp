#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
	//ofSetFrameRate(120);
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
    
    //    iRadius_ =200;
    //    a_ = 1;
    //    b_ = 1;
    //    m_ = 2;
    //    n1_ =20;
    //    n2_ = 1;
    //    n3_ = 1;
    
    iRadius_ =170;
    a_ = 0.24;
    b_ = 0.34;
    m_ = 11.92;
    n1_ =12.81;
    n2_ = -2.71;
    n3_ = 2.46;
    
    stkWeight_ = 3.5;
    
    cam.enableMouseInput();
    
    
    setupGUI();
    setupShaper();
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
    gui->addSlider("STROKE",    0, 5, &stkWeight_);
    gui->addSpacer();
    gui->addButton("SAVE SHAPE", false);
    gui->addButton("DRAW CIRCLE", false);
    gui->addButton("DRAW SUPERSHAPE", false);
    gui->addButton("DRAW SUPERSHAPE FFT", false);
    gui->addButton("DRAW SUPERSHAPE 3D", false);
    gui->autoSizeToFitWidgets();
    
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
}
void testApp::setupShaper(){
    
    iRadius.setTarget(iRadius_);
    
    
    a.setTarget(a_);
    b.setTarget(b_);
    m.setTarget(m_);
    n1.setTarget(n1_);
    n2.setTarget(n2_);
    n3.setTarget(n3_);
    
    stkWeight.setTarget(stkWeight_);
    
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
    if(name == "SAVE SHAPE"){
        ofxUIButton* b = (ofxUIButton*)e.widget;
        if(b->getValue()){
            toMesh.push_back(temp);
            cout<<toMesh.size()<<endl;
        }
        
    }
    if(name == "DRAW CIRCLE"){
        cout<<"setting mode : circle"<<endl;
        currentMode = circle;
    }
    if(name == "DRAW SUPERSHAPE"){
        currentMode = supershape;
        setupShaper();
        cout<<"setting mode : supershape"<<endl;
    }
    if(name == "DRAW SUPERSHAPE 3D"){
        superShapeFFT();
        currentMode = supershapeFFT3D;
        
        
        cout<<"setting mode : supershapeFFT"<<endl;
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
            cout<<fftMax<<endl;
            
		}
		if(msgIn.getAddress() == "/saveFrame") {
            toMesh.push_back(temp);
			string filename = msgIn.getArgAsString(0);
            saveFrame = true;
            
		}
		if(msgIn.getAddress() == "/Switch") {
		}
		if(msgIn.getAddress() == "/radCurve") {
            radCurve    = msgIn.getArgAsFloat(0);
            
		}
		if(msgIn.getAddress() == "/duration/info") {
            //			string info = msgIn.getArgAsString(0);
            //            cout<<info<<endl;
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
    
    ofSetLineWidth(2);
    ofNoFill();
    
    ofSetColor(ofColor::white);
    
    glPushMatrix();
    
    
    if(currentMode == supershape){
        ofSetLineWidth(stkWeight.value);
        superShape();
    }
    else if(currentMode == circle){
        if(mesh.getNumVertices()> 0){
            ofPushMatrix();
            cam.begin();
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            for (int i=0; i<mesh.getNumVertices(); i++) {
                
                ofSphere(mesh.getVertices()[i].x, mesh.getVertices()[i].y, mesh.getVertices()[i].z, 3);
            }
            cam.end();
            ofPopMatrix();
        }
        //        circleFFT();
    }
    else if(currentMode == supershapeFFT3D){
        if(mesh.getVertices().size()> 0){
            ofPushMatrix();
            cam.begin();
            /*
             ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
             //            ofRotate(ofGetMouseY(), 1, 0, 0);
             //	mainMesh.drawFaces();
             cout<<"drawing"<<endl;
             //
             GLuint vbo;
             glGenBuffers( 1, &vbo );
             glBindBuffer( GL_ARRAY_BUFFER, vbo );
             glBufferData( GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(ofVec3f), mesh.getVertices().data(), GL_STATIC_DRAW );
             
             glDrawArrays( GL_POINTS, 0, mesh.getVertices().size() );
             
             glDeleteBuffers(1, &vbo);
             glFlush();
             
             */
            
            //            for (int i=0; i<mesh.getNumVertices(); i++) {
            //
            //                ofSphere(mesh.getVertices()[i].x, mesh.getVertices()[i].y, mesh.getVertices()[i].z, 10);
            //            }
            mesh.drawWireframe();
            cam.end();
            ofPopMatrix();
            
        }
        
    }
    
    glPopMatrix();
    
    if(saveFrame){
        ofSaveFrame();
        cout<<"savingFrame!"<<endl;
        saveFrame = false;
    }
    
    
}
void testApp:: circleFFT(){
    if(fftData.size() > 0){
        ofBeginShape();
        for (int i = 0; i <fftData.size(); i++){
            float val =fftMax - fftData[i] * 128;
        }
        
        float x,y;
        float radius = 100;
        float centX = ofGetWidth()/2;
        float centY = -ofGetHeight()/2;
        
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
        
    }
    
}
void testApp::superShapeFFT(){
    if(toMesh.size()>0){
        mesh.clear();
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        cout<<"Making mesh"<<endl;
        
        //lay the slices nicely in z
        for (int i=0; i < toMesh.size(); i++) {
            //TODO: make the max z a parameter
            
            float zVal = ofMap(i, 0, toMesh.size(), 0, 500);
            cout<<" no. of vertices in  "<<ofToString(i)<<" = "<<toMesh[i].size();
            
            for(int j=0; j<toMesh[i].size(); j++){
                toMesh[i][j].z = zVal;
            }
        }
        
        //add vertices to the mesh
        for (int i =0; i< toMesh.size(); i++){
            for(int j= 0; j<toMesh[i].size(); j++)
            {
                mesh.addVertex(toMesh[i][j]  );
            }
        }
        
        //add index to the mesh
        for (int y =0; y< toMesh.size() -1; y++){
            int height = toMesh.size();
            for(int x= 0; x<toMesh[y].size()-1; x++)
            {
                int width = toMesh[y].size();
                
                mesh.addIndex( x     +       (y * width  ));           //0
                mesh.addIndex( x     +      ((y +1)  * width ));       //1
                mesh.addIndex((x +1) +      ( y *width) );        //10
                
                mesh.addIndex( x      +     ( (y+ 1) *width));       //1
                mesh.addIndex((x +1) +      ( (y+1) * width ));    //11
                mesh.addIndex((x +1) +      ( y * width ));    //10
            }
        }
        
        
        //
        cout<<"Mesh has : "<<mesh.getNumVertices()<<" vertices "<<endl;
    }
}
void testApp::superShape(){
    temp.clear();
    ofBeginShape();
    
    for(float theta = 0; theta <TWO_PI+0.001f; theta+=0.003f)
    {
        float raux  = pow(abs(1.0f/a.value)*abs(cos((m.value*theta/4.0f))),n2.value) + pow(abs(1.0f/b.value)*abs(sin(m.value*theta/4.0f)),n3.value);
        float r = iRadius.value*pow(abs(raux),(-1.0f/n1.value));
        if(bFFT){
            
            int index = ofMap(theta, 0, TWO_PI+0.001f, 0, fftData.size());
            float hue = ofMap(index, 0, fftData.size(), 0, 255);
            float radVariance =ofMap(fftData[index], 0, fftMax, 0, 1);
            r+=  radVariance*100;
        }
        
        float x=ofGetWidth()*.5f+r*cos(theta);
        float y=ofGetHeight()*.5f+r*sin(theta);
        
        temp.push_back(ofVec3f(x,y,0));
        ofCurveVertex(x,y);
    }
    
    ofEndShape();
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
