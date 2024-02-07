#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);
	ofBackground(255, 255, 255);

	oscPatchboard.loadFile(oscPatchboardFile);

	if (oscPatchboard.inputType != "OSC")
	{
		cout << "Reading: oscPatchboard.inputType = " << oscPatchboard.inputType << endl;
		cout << "ERROR: Specify OSC as input type in " << oscPatchboardFile << endl;
		while (true);
	}
	string portStr = oscPatchboard.patchboard.getValue("patchboard:settings:input:port", "");
	if (portStr == "")
	{
		cout << "ERROR: Specify patchboard:settings:input:port in " << oscPatchboardFile << endl;
		while (true);
	}
	int port = ofToInt(portStr);
	receiver.setup(port);
	
	scopeWins = ofxMultiScope::loadScopeSettings();
	plotIdIndexes = ofxMultiScope::getPlotIdIndexes();
	if (scopeWins.size() == 0) {
		cout << "ABORTING: No ofxOscilloscope settings found..." << endl;
		while(true);
	}

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
	while (receiver.hasWaitingMessages()) {

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
		string messageAddress = m.getAddress();

		// Iterate through patchcords to see if we're plotting any of the incoming data
		// ToDo: Use unordered_map.find()
		for (auto patch = oscPatchboard.patchcords.begin(); patch != oscPatchboard.patchcords.end(); ++patch)
		{
			// ToDo: Handle cases where specific data array indexes go to specific plots
			string patchAddress = patch->first;
			if (messageAddress.compare(patchAddress) == 0) // if we're plotting this address!
			{
				if (recordData)
				{
					for (size_t i = 0; i < m.getNumArgs(); i++)
					{
						dataLogger.push(m.getAddress() + ',' 
							+ ofToString((ofGetElapsedTimeMillis() - recordDataStartTime) / 1000.f) 
							+ ',' + ofToString(m.getArgAsFloat(i)) + "\n");
					}
				}

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
			else
			{
				bool heyThere = true;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	for (int w = 0; w < scopeWins.size(); w++)
	{
		scopeWins.at(w).plot();
	}
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
	cout << "Key Released: " << (char) key << "\n";

	if (key == 32) { // Space Bar
		isPaused = !isPaused;
	}


	if (key == 'S') {
		ofxMultiScope::saveScopeSettings(scopeWins);
	}
	if (key == 'L') {
		scopeWins = ofxMultiScope::loadScopeSettings();
		plotIdIndexes = ofxMultiScope::getPlotIdIndexes();
	}
	if (key == 'P') {
		oscPatchboard.loadFile(oscPatchboardFile);
		for (int w = 0; w < scopeWins.size(); w++) {
			scopeWins.at(w).clearData();
		}
	}
	if (key == 'R')
	{
		cout << "Recording OSC Data: ";
		string localTime = ofGetTimestampString("%Y-%m-%d_%H-%M-%S-%f");
		//string localTime = ofGetTimestampString();
		string filename = localTime + ".csv";
		cout << filename << endl;
		dataLogger.setFilename(filename);
		recordDataStartTime = ofGetElapsedTimeMillis();
		recordData = true;
		dataLogger.startThread();
	}
	if (key == 'r')
	{
		cout << "Stopping OSC Data Recording";
		recordData = false;
		dataLogger.stopThread();
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


string ofApp::ofGetTimestampString(const string& timestampFormat) {
	std::stringstream str;
	auto now = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(now);    std::chrono::duration<double> s = now - std::chrono::system_clock::from_time_t(t);
	int us = s.count() * 1000000;
	auto tm = *std::localtime(&t);
	constexpr int bufsize = 256;
	char buf[bufsize];

	// Beware! an invalid timestamp string crashes windows apps.
	// so we have to filter out %i (which is not supported by vs)
	// earlier.
	auto tmpTimestampFormat = timestampFormat;
	ofStringReplace(tmpTimestampFormat, "%i", ofToString(us / 1000, 3, '0'));
	ofStringReplace(tmpTimestampFormat, "%f", ofToString(us, 6, '0'));

	if (strftime(buf, bufsize, tmpTimestampFormat.c_str(), &tm) != 0) {
		str << buf;
	}
	auto ret = str.str();


	return ret;
}