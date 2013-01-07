//
//  oscilloscope.h
//
//  Created by Sean Montgomery on 12/21/12.
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//

#ifndef _OSCILLOSCOPE
#define _OSCILLOSCOPE

#include "ofMain.h"

/*-------------------------------------------------
* ofxScopePlot
* Class to handle the plotting of data for ofxOscilloscope
*-------------------------------------------------*/
class ofxScopePlot {
private:
	ofPoint _min;
	ofPoint _max;
	float _timeWindow;
	int _pointsPerWin; //(nPoints)
	int _sampFreq;
	int _nVariables;
	ofColor * _variableColors;
	float ** _buffer;
	float _yScale;
	float _yOffset;
public:
	ofxScopePlot();
	~ofxScopePlot();
	void setup(float timeWindow, int sampFreq, 
		ofColor variableColors[], int nVariables, 
		ofPoint min, ofPoint max, float yScale=1.0, float yOffset=0.0);
	void setVariableColors(ofColor colors[], int nColors);
	void updateData(float ** data, int nPoints); // data[_nVariables][nPoints]
	void plot();

	int getNumVariables();
	ofColor getVariableColor(int i);
	float getTimeWindow();
};

class ofxOscilloscope {
private:
	ofPoint _min;
	ofPoint _max;
	int _legendWidth;
	ofTrueTypeFont _legendFont;
	int _legendPadding;
	int _textSpacer;
	ofxScopePlot _scope;
	string * _variableNames;
public:
	//ofxOscilloscope();
	ofxOscilloscope(ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(), int legendWidth=100);
	~ofxOscilloscope();
	void setup(float timeWindow, int sampFreq, 
		string variableNames[], ofColor variableColors[], int nVariables, 
		float yScale=1.0, float yOffset=0.0);
	void setVariableNames(string variableNames[], int nVariables);
	void setVariableColors(ofColor colors[], int nColors);
	void updateData(float ** data, int nPoints); // data[_nVariables][nPoints]
	void plot();
	void setTimeWindow(float timeWindow);

	string getVariableName(int i);
};

class ofxMultiScope {
private:
	ofPoint _min;
	ofPoint _max;
	int _numScopes;
public:
	ofxOscilloscope * scopes;

	ofxMultiScope();
	ofxMultiScope(int numScopes, ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(), int legendWidth=100);
	~ofxMultiScope();
	void setTimeWindow(float timeWindow);
	void setSize(ofPoint min, ofPoint max);
	//void updateData(int scopeNum, float ** data, int nPoints);
	void plot();
};

#endif