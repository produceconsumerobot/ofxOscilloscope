#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);

	patchboard.loadFile(patchboardFile);

	if (patchboard.settings.input["type"].compare("OSC") != 0)
	{
		cout << "ERROR: Specify OSC as input type in " << patchboardFile << endl;
		while (true);
	}
		
	int port = ofToInt(patchboard.settings.input["port"]);
	receiver.setup(port);
	
	scopeWins = ofxMultiScope::loadScopeSettings();
	plotIdIndexes = ofxMultiScope::getPlotIdIndexes();
	if (scopeWins.size() == 0) {
		cout << "ABORTING: No ofxOscilloscope settings found..." << endl;
		while(true);
	}

	//scopeWin = scopeWins.at(0);

	// Allocate space for new data in form data[nVariables][nPoints]
	const int nVariables = 2;
	newPoints = 23;
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
	//if (zeroData) {
	//	data.at(0).assign(data.at(0).size(), 0.);
	//}
	//else {
	//	for (int i = 0; i<newPoints; i++) {
	//		data.at(0).at(i) = counter; counter++; if (counter>ofGetWindowSize().y / 2) counter = -ofGetWindowSize().y / 2;// + ofRandomf()
	//	}
	//}
	//for (int i = 0; i<newPoints; i++) {
	//	data.at(1).at(i) = counter2; counter2--; if (counter2<-50) counter2 = 50;// + ofRandomf()
	//}

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

	while (receiver.hasWaitingMessages()) {

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);



		// Iterate through patchcords to see if we're plotting any of the incoming data
		for (auto patch = patchboard.patchcords.begin(); patch != patchboard.patchcords.end(); ++patch)
		{
			// ToDo: Handle cases where specific data array indexes go to specific plots
			string address = patch->first;
			if (m.getAddress().compare(address) == 0) // if we're plotting this address!
			{
				for (int pcord = 0; pcord < patch->second.size(); pcord++)
				{
					// plot it to the specified plotId(s)
					vector<size_t> ind = plotIdIndexes[ofToInt(patch->second.at(pcord))];
					if (ind.size() > 0)
					{
						vector<float> data;
						int w = ind.at(0); // ScopeWin
						int s = ind.at(1); // Scope
						int v = ind.at(2); // Variable/Plot
						for (size_t i = 0; i < m.getNumArgs(); i++)
						{
							data.clear();
							data.push_back(m.getArgAsFloat(i));
							scopeWins.at(w).scopes.at(s).updateData(v, data);
						}
					}
				}
			}
		}
	}

	//// Add data to the scopes
	//if (!isPaused) {
	//	for (int i = 0; i< scopeWin.scopes.size(); i++) {
	//		scopeWin.scopes.at(i).updateData(data);
	//		//scopeWin.scopes.at(i).updateData(array_data, data.at(0).size());
	//	}
	//}

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

	for (int w = 0; w < scopeWins.size(); w++)
	{
		scopeWins.at(w).plot();
	}
	//SleepMillis(100);
}

//--------------------------------------------------------------
void ofApp::exit() {
	printf("exit()");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	cout << "Key Released: " << key << "\n";

	if (key == 32) { // Space Bar
		isPaused = !isPaused;
	}


	if (key == 'S') {
		ofxMultiScope::saveScopeSettings(scopeWins);
	}
	if (key == 'L') {
		vector<ofxMultiScope> scopeWins = ofxMultiScope::loadScopeSettings();
		plotIdIndexes = ofxMultiScope::getPlotIdIndexes();
	}
	if (key == 'P') {
		patchboard.loadFile(patchboardFile);
		for (int w = 0; w < scopeWins.size(); w++) {
			scopeWins.at(w).clearData();
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
