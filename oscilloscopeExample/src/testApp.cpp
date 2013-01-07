#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

	nVariables = 2;
	newPoints = 23;
	
	ofColor colors[2] = {ofColor(0,0,0), ofColor(255,0,0)};
	string names[2] = {"happy", "joy"};

	nScopes = 5;
	min = ofPoint(0., 0.);
	max = ofPoint(ofGetWindowSize().x, ofGetWindowSize().y);
	scopeWin = ofxMultiScope(nScopes, min, max);
	for (int i=0; i<nScopes; i++) {
		scopeWin.scopes[i].setup(5, (i+1)*100, names, colors, nVariables, 1., 0.);
	}

	data = new float*[nVariables];
	for (int i=0; i<nVariables; i++) {
		data[i] = new float[newPoints];
	}

	zeroData = false;
	counter = 0;
	counter2 = 0;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	//printf("draw()\n");

	if (zeroData) {
		for (int i=0; i<newPoints; i++) {
			data[0][i] = 0.;
		}
	} else {
		for (int i=0; i<newPoints; i++) {
			data[0][i] = counter; counter++; if(counter>ofGetWindowSize().y/2) counter=-ofGetWindowSize().y/2;// + ofRandomf()
		}
	}
	for (int i=0; i<newPoints; i++) {
		data[1][i] = counter2; counter2--; if(counter2<-50) counter2=50;// + ofRandomf()
	}


	for (int i=0; i<nScopes; i++) {
		scopeWin.scopes[i].updateData(data, newPoints);
	}
	zeroData = false;
	scopeWin.plot();
	ofSleepMillis(100);
}

//--------------------------------------------------------------
void testApp::exit(){
	printf("exit()");
	for (int i=0; i<nVariables; i++) {
		delete data[i];
	}
	delete data;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == '0') {
		zeroData = true;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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