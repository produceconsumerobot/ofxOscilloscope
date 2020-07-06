#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofTrueTypeFont legendFont;
	legendFont.load("verdana.ttf", 12, true, true);

	const int nVariables = 2;
	newPoints = 23;

	ofColor colors[nVariables] = { ofColor(0,0,0), ofColor(255,0,0) };
	std::vector<ofColor> vec_colors(colors, colors + nVariables);

	string names[2] = { "happy", "joy" };
	std::vector<string> vec_names(names, names + nVariables);

	nScopes = 5;
	float timeWindow = 5.; // seconds
	float samplingFreqMult = 100;
	float yScale = 1.; // yScale multiplier
	float yOffset = 0.; // yOffset from the center of the scope window
	ofRectangle scopeArea = ofRectangle(ofPoint(0, 0), ofGetWindowWidth(), ofGetWindowHeight());

	scopeWin = ofxMultiScope(nScopes, scopeArea, legendFont); // Setup the multiScope panel

	for (int i = 0; i<nScopes; i++) {
		// Setup the oscilloscopes
		scopeWin.scopes.at(i).setup(timeWindow, (i + 1)*samplingFreqMult, vec_names, vec_colors,
			yScale, yOffset); // Setup each oscilloscope panel
		scopeWin.scopes.at(i).autoscaleY(autoscaleY);
	}

	// Allocate space for new data in form data[nVariables][nPoints]
	data.resize(nVariables, vector<float>(newPoints, 0));

	zeroData = false;
	counter = 0;
	counter2 = 0;

	selectedScope = 0; // Select all scopes for increment/decrement

	isPaused = false;
}

//--------------------------------------------------------------
void ofApp::update() {
	// Generate dummy data
	if (zeroData) {
		data.at(0).assign(data.at(0).size(), 0.);
	}
	else {
		for (int i = 0; i<newPoints; i++) {
			data.at(0).at(i) = counter; counter++; if (counter>ofGetWindowSize().y / 2) counter = -ofGetWindowSize().y / 2;// + ofRandomf()
		}
	}
	for (int i = 0; i<newPoints; i++) {
		data.at(1).at(i) = counter2; counter2--; if (counter2<-50) counter2 = 50;// + ofRandomf()
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

	// Add data to the scopes
	if (!isPaused) {
		for (int i = 0; i<nScopes; i++) {
			scopeWin.scopes.at(i).updateData(data);
			//scopeWin.scopes.at(i).updateData(array_data, data.at(0).size());
		}
	}

	/*
	// Code for Testing arrays...
	// DON'T USE ARRAYS UNLESS YOU WANT A HEADACHE
	for (int i=0; i<data.size(); i++) {
	delete[] array_data[i];
	}
	delete[] array_data;
	*/
}

//--------------------------------------------------------------
void ofApp::draw() {
	//printf("draw()\n");

	scopeWin.plot();
	ofSleepMillis(100);
}

//--------------------------------------------------------------
void ofApp::exit() {
	printf("exit()");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Increment the yScale
	if (key == '+') {
		if (selectedScope == 0) {
			scopeWin.incrementYScale();
		}
		else {
			scopeWin.scopes.at(selectedScope - 1).incrementYScale();
		}
	}

	// Decrement the yScale
	if ((key == '-') || (key == '_')) {
		if (selectedScope == 0) {
			scopeWin.decrementYScale();
		}
		else {
			scopeWin.scopes.at(selectedScope - 1).decrementYScale();
		}
	}

	// Increment the yOffset
	if (key == 357) { // Up Arrow
		if (selectedScope == 0) {
			scopeWin.incrementYOffset();
		}
		else {
			scopeWin.scopes.at(selectedScope - 1).incrementYOffset();
		}
	}

	// Decrement the yOffset
	if (key == 359) { // Down Arrow
		if (selectedScope == 0) {
			scopeWin.decrementYOffset();
		}
		else {
			scopeWin.scopes.at(selectedScope - 1).decrementYOffset();
		}
	}

	// Increment the timeWindow
	if (key == 358) { // Right Arrow
		if (selectedScope == 0) {
			scopeWin.incrementTimeWindow();
		}
		else {
			scopeWin.scopes.at(selectedScope - 1).incrementTimeWindow();
		}
	}

	// Decrement the timeWindow
	if (key == 356) { // Left Arrow
		if (selectedScope == 0) {
			scopeWin.decrementTimeWindow();
		}
		else {
			scopeWin.scopes.at(selectedScope - 1).decrementTimeWindow();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	cout << "Key Released: " << key << "\n";

	// Choose an oscilloscope panel for changing yScale/yOffset/timeWindow
	// Starts counting from 1
	// Zero = all
	if ((key >= 48) && (key <= 57)) {
		int number = key - 48;
		if (number <= nScopes) {
			selectedScope = number;
			scopeWin.setOutlineWidth(1);
			if (selectedScope > 0) {
				scopeWin.scopes.at(selectedScope - 1).setOutlineWidth(5);
			}
		}
	}

	if (key == 32) { // Space Bar
		isPaused = !isPaused;
	}


	// testing line widths
	if (key == ',') {
		scopeWin.scopes.at(1).setOutlineWidth(10);
		scopeWin.scopes.at(1).setPlotLineWidth(5);
	}
	if (key == '.') {
		scopeWin.scopes.at(1).setOutlineWidth(1);
		scopeWin.scopes.at(1).setPlotLineWidth(1);
	}


	// testing setVariableNames
	if (key == 'n') {
		const int nVariables = 2;
		string names[nVariables] = { "happy", "joy" };
		std::vector<string> vec_names(names, names + nVariables);
		scopeWin.scopes.at(1).setVariableNames(vec_names);
	}
	if (key == 'm') {
		const int nVariables = 2;
		string names[nVariables] = { "wow", "zing" };
		std::vector<string> vec_names(names, names + nVariables);
		scopeWin.scopes.at(1).setVariableNames(vec_names);
	}

	//testing setVariableColors
	if (key == 'z') {
		const int nVariables = 2;
		ofColor colors[nVariables] = { ofColor(0,0,0), ofColor(255,0,0) };
		std::vector<ofColor> vec_colors(colors, colors + nVariables);
		scopeWin.scopes.at(1).setVariableColors(vec_colors);
	}
	if (key == 'x') {
		const int nVariables = 2;
		ofColor colors[nVariables] = { ofColor(0,255,0), ofColor(255,255,0) };
		std::vector<ofColor> vec_colors(colors, colors + nVariables);
		scopeWin.scopes.at(1).setVariableColors(vec_colors);
	}

	//testing setTimeWindow
	if (key == 't') {
		scopeWin.setTimeWindow(5);
	}
	if (key == 'y') {
		scopeWin.setTimeWindow(10);
	}
	if (key == 'u') {
		scopeWin.scopes.at(1).setTimeWindow(15);
	}

	//testing setPosition
	if (key == 'a') {
		ofRectangle pos = scopeWin.scopes.at(1).getPosition();
		pos.setX(pos.getX() + 50);
		scopeWin.scopes.at(1).setPosition(pos);
	}
	if (key == 's') {
		ofRectangle pos = scopeWin.getPosition();
		pos.setX(pos.getX() + 50);
		scopeWin.setPosition(pos);
	}
	if (key == 'd') {
		min = ofPoint(0., 0.);
		max = ofPoint(ofGetWindowSize().x, ofGetWindowSize().y);
		scopeWin.setPosition(min, max);
	}
	if (key == 'f') {
		min = ofPoint(50., 300.);
		max = ofPoint(ofGetWindowSize().x - 200., ofGetWindowSize().y - 200.);
		scopeWin.setPosition(min, max);
	}

	// testing backgroundColor
	if (key == 'c') {
		ofColor color = ofColor(0, 0, 0, 0.);
		scopeWin.setBackgroundColor(color);
	}
	if (key == 'v') {
		ofColor color = ofColor(0, 255, 0, 128);
		scopeWin.setBackgroundColor(color);
	}
	if (key == 'b') {
		ofColor color = ofColor(0, 0, 255, 128);
		scopeWin.scopes.at(1).setBackgroundColor(color);
	}

	// testing setOutlineColor
	if (key == 'j') {
		ofColor color = ofColor(0, 0, 0, 255);
		scopeWin.setOutlineColor(color);
	}
	if (key == 'k') {
		ofColor color = ofColor(0, 255, 0, 128);
		scopeWin.setOutlineColor(color);
	}
	if (key == 'l') {
		ofColor color = ofColor(0, 0, 255, 128);
		scopeWin.scopes.at(1).setOutlineColor(color);
	}

	// testing setZeroLineColor
	if (key == 'i') {
		ofColor color = ofColor(0, 0, 0, 255);
		scopeWin.setZeroLineColor(color);
	}
	if (key == 'o') {
		ofColor color = ofColor(0, 255, 0, 128);
		scopeWin.setZeroLineColor(color);
	}
	if (key == 'p') {
		ofColor color = ofColor(0, 0, 255, 128);
		scopeWin.scopes.at(1).setZeroLineColor(color);
	}

	// testing setLegendFont
	if (key == 'g') {
		ofTrueTypeFont legendFont;
		legendFont.loadFont("verdana.ttf", 12, true, true);
		legendFont.setLetterSpacing(0.7);
		scopeWin.setLegendFont(legendFont);
	}
	if (key == 'h') {
		ofTrueTypeFont legendFont;
		legendFont.loadFont("verdana.ttf", 12, true, true);
		legendFont.setLineHeight(15.0f);
		legendFont.setLetterSpacing(1.5);
		scopeWin.scopes.at(1).setLegendFont(legendFont);
	}

	// testing setLegendWidth
	if (key == 'q') {
		scopeWin.setLegendWidth(75);
	}
	if (key == 'w') {
		scopeWin.scopes.at(1).setLegendWidth(200);
	}

	// testing setTextSpacing
	if (key == 'e') {
		scopeWin.setTextSpacing(15, 15);
	}
	if (key == 'r') {
		scopeWin.scopes.at(1).setTextSpacing(20, 10);
	}
	if (key == ':') {
		autoscaleY = !autoscaleY;
		for (int i = 0; i<nScopes; i++) {
			scopeWin.scopes.at(i).autoscaleY(autoscaleY);
		}
	}
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}