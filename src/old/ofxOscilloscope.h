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
#include <vector>
#include <algorithm>


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
	std::vector<ofColor> _variableColors;
	std::vector< vector<float > > _buffer;
	float _yScale;
	float _yOffset;
	ofColor _zeroLineColor;
public:
	ofxScopePlot();
	~ofxScopePlot();

	void setup(float timeWindow, int sampFreq, std::vector<ofColor> variableColors, 
		ofPoint min, ofPoint max, float yScale=1.0, float yOffset=0.0,
		ofColor zeroLineColor=ofColor(240,240,240));
	void setup(float timeWindow, int sampFreq, ofColor variableColors[], int nVariables, 
		ofPoint min, ofPoint max, float yScale=1.0, float yOffset=0.0, 
		ofColor zeroLineColor=ofColor(240,240,240));

	void setVariableColors(std::vector<ofColor> colors);
	void setVariableColors(ofColor colors[], int nColors);

	void setZeroLineColor(ofColor zeroLineColor);

	void updateData(float ** data, int nPoints); // data[_nVariables][nPoints]
	void updateData(std::vector<std::vector<float> > data);
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
	std::vector<string> _variableNames;
	ofColor _outlineColor;
public:
	ofxOscilloscope(ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(), int legendWidth=100,
		ofColor outlineColor=ofColor(200,200,200), ofColor zeroLineColor=ofColor(240,240,240));
	~ofxOscilloscope();

	void setup(float timeWindow, int sampFreq, 
		std::vector<string> variableNames, std::vector<ofColor> variableColors, 
		float yScale=1.0, float yOffset=0.0);
	void setup(float timeWindow, int sampFreq, 
		string variableNames[], ofColor variableColors[], int nVariables, 
		float yScale=1.0, float yOffset=0.0);

	void setVariableNames(std::vector<string> variableNames);
	void setVariableNames(string variableNames[], int nVariables);

	void setVariableColors(std::vector<ofColor> colors);
	void setVariableColors(ofColor colors[], int nColors);

	void setOutlineColor(ofColor outlineColor);
	void setZeroLineColor(ofColor zeroLineColor);

	void updateData(std::vector<std::vector<float> > data);
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
	std::vector<ofxOscilloscope> scopes;

	ofxMultiScope();
	ofxMultiScope(int numScopes, ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(), int legendWidth=100);
	~ofxMultiScope();
	void setTimeWindow(float timeWindow);
	void setSize(ofPoint min, ofPoint max);
	//void updateData(int scopeNum, float ** data, int nPoints);
	void plot();
};

#endif
