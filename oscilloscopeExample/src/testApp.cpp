#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

	const int nVariables = 2;
	newPoints = 23;
	
	ofColor colors[nVariables] = {ofColor(0,0,0), ofColor(255,0,0)};
	std::vector<ofColor> vec_colors(colors, colors + nVariables);

	string names[2] = {"happy", "joy"};
	std::vector<string> vec_names(names, names + nVariables);

	nScopes = 5;
	min = ofPoint(0., 0.);
	max = ofPoint(ofGetWindowSize().x, ofGetWindowSize().y);
	scopeWin = ofxMultiScope(nScopes, min, max);

	for (int i=0; i<nScopes; i++) {
		// Setup the oscilloscopes
		scopeWin.scopes.at(i).setup(5, (i+1)*100, vec_names, vec_colors, 1., 0.);
	}

	// Allocate space for new data in form data[nVariables][nPoints]
	data.resize(nVariables, vector<float>(newPoints, 0));

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
		data.at(0).assign(data.at(0).size(), 0.);
	} else {
		for (int i=0; i<newPoints; i++) {
			data.at(0).at(i) = counter; counter++; if(counter>ofGetWindowSize().y/2) counter=-ofGetWindowSize().y/2;// + ofRandomf()
		}
	}
	for (int i=0; i<newPoints; i++) {
		data.at(1).at(i) = counter2; counter2--; if(counter2<-50) counter2=50;// + ofRandomf()
	}

	/*
	// Code for Testing arrays... 
	// DON'T USE ARRAYS UNLESS YOU WANT A HEADACHE
	float ** array_data = new float*[data.size()];
	for (int i=0; i<data.size(); i++) {
		array_data[i] = new float[data.at(i).size()];
		for (int j=0; j<data.at(i).size(); j++) {
			array_data[i][j] = data.at(i).at(j);
		}
	}
	*/

	for (int i=0; i<nScopes; i++) {
		scopeWin.scopes.at(i).updateData(data);
		//scopeWin.scopes.at(i).updateData(array_data, data.at(0).size());
	}

	/*
	// Code for Testing arrays... 
	// DON'T USE ARRAYS UNLESS YOU WANT A HEADACHE
	for (int i=0; i<data.size(); i++) {
		delete[] array_data[i];
	}
	delete[] array_data;
	*/

	zeroData = false;
	scopeWin.plot();
	ofSleepMillis(100);
}

//--------------------------------------------------------------
void testApp::exit(){
	printf("exit()");
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