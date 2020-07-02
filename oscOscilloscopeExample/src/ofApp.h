#pragma once

#include "ofMain.h"
#include "ofxOscilloscope.h"
#include "ofxOsc.h"
#include "patchboard.h"
#include "ofxThreadedLogger.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	string ofGetTimestampString(const string& timestampFormat);

	//ofxMultiScope scopeWin;
	int newPoints;
	int nScopes;
	std::vector<std::vector<float> > data;
	bool zeroData;
	int counter;
	int counter2;
	ofPoint min;
	ofPoint max;

	int selectedScope;

	bool isPaused;
	bool autoscaleY = false;

	ofxOscReceiver receiver;
	string patchboardFile = "oscInputSettings.xml";
	Patchboard patchboard;

	vector<ofxMultiScope> scopeWins;
	unordered_map<int, vector<size_t>> plotIdIndexes;

	bool recordData;
	uint64_t recordDataStartTime;
	LoggerThread dataLogger;

};
