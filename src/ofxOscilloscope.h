//
//  ofxOscilloscope.h
//
//  openFrameworks addOn to display data as on an oscilloscope
//
//  Created by Sean Montgomery on 12/21/12.
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//

#ifndef _OFX_OSCILLOSCOPE
#define _OFX_OSCILLOSCOPE

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
	float _sampFreq;
	int _nVariables;
	std::vector<ofColor> _variableColors;
	std::vector< vector<float > > _buffer;
	float _yScale;
	float _yOffset;
	ofColor _zeroLineColor;
	ofColor _backgroundColor;
public:
	// Constructors
	ofxScopePlot(ofRectangle plotArea,
		ofColor zeroLineColor=ofColor(240,240,240), ofColor backgroundColor=ofColor(0.,0.,0.,0.));
	ofxScopePlot(ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(),
		ofColor zeroLineColor=ofColor(240,240,240), ofColor backgroundColor=ofColor(0.,0.,0.,0.));
	// Destructor
	~ofxScopePlot();

	// Setup
	void setup(float timeWindow, float sampFreq, std::vector<ofColor> variableColors, 
		float yScale=1.0, float yOffset=0.0);
	void setup(float timeWindow, float sampFreq, ofColor variableColors[], int nVariables, 
		float yScale=1.0, float yOffset=0.0);

	// Updating data
	void updateData(std::vector<float> data); // data[_nVariables]
	void updateData(std::vector<std::vector<float> > data); // data[_nVariables][nPoints]
	void updateData(float ** data, int nPoints); // ** DEPRECATED ** data[_nVariables][nPoints]

	// Plotting
	void plot();

	// Setters/Getters
	void setVariableColors(std::vector<ofColor> colors);
	void setVariableColors(ofColor colors[], int nColors);

	void setZeroLineColor(ofColor zeroLineColor);
	void setBackgroundColor(ofColor backgroundColor);

	void setPosition(ofPoint min, ofPoint max);
	void setPosition(ofRectangle plotArea);
	ofRectangle getPosition();

	void setTimeWindow(float timeWindow);

	void setYScale(float yScale);
	float getYScale();
	void setYOffset(float yOffset);
	float getYOffset();

	int getNumVariables();
	ofColor getVariableColor(int i);
	float getTimeWindow();
};



/*-------------------------------------------------
* ofxOscilloscope
* Class to handle the plotting of data on an oscilloscope panel.
* Multiple data streams can be plotted on a single panel.
*-------------------------------------------------*/
class ofxOscilloscope {
private:
	ofPoint _min;
	ofPoint _max;
	int _legendWidth;
	ofTrueTypeFont _legendFont;
	int _legendPadding;
	int _textSpacer;
	ofxScopePlot _scopePlot;
	std::vector<string> _variableNames;
	ofColor _outlineColor;
	ofColor _backgroundColor;
public:
	// Constructors
	ofxOscilloscope(ofRectangle scopeArea, 
		ofTrueTypeFont legendFont=ofTrueTypeFont(), int legendWidth=100,
		ofColor outlineColor=ofColor(200,200,200), ofColor zeroLineColor=ofColor(240,240,240),
		ofColor backgroundColor=ofColor(0.,0.,0.,0.));
	ofxOscilloscope(ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(), 
		ofTrueTypeFont legendFont=ofTrueTypeFont(), int legendWidth=100,
		ofColor outlineColor=ofColor(200,200,200), ofColor zeroLineColor=ofColor(240,240,240),
		ofColor backgroundColor=ofColor(0.,0.,0.,0.));
	// Destructor
	~ofxOscilloscope();

	// Setup of scope
	void setup(float timeWindow, float sampFreq, 
		std::vector<string> variableNames, std::vector<ofColor> variableColors, 
		float yScale=1.0, float yOffset=0.0);
	void setup(float timeWindow, float sampFreq, 
		string variableNames[], ofColor variableColors[], int nVariables, 
		float yScale=1.0, float yOffset=0.0);

	// Updating data
	void updateData(std::vector<float> data); // data[_nVariables]
	void updateData(std::vector<std::vector<float> > data); // data[_nVariables][nPoints]
	void updateData(float ** data, int nPoints); // ** DEPRECATED ** data[_nVariables][nPoints] 

	// Plotting
	void plot();

	// Setters/Getters
	void setVariableNames(std::vector<string> variableNames);
	void setVariableNames(string variableNames[], int nVariables);

	void setVariableColors(std::vector<ofColor> colors);
	void setVariableColors(ofColor colors[], int nColors);

	void setTimeWindow(float timeWindow);
	void setPosition(ofPoint min, ofPoint max);
	void setPosition(ofRectangle scopeArea);
	ofRectangle getPosition();
	//void setPosition(ofPoint center, int height, int width);

	void setOutlineColor(ofColor outlineColor);
	void setZeroLineColor(ofColor zeroLineColor);
	void setBackgroundColor(ofColor backgroundColor);

	void setLegendFont(ofTrueTypeFont legendFont);
	void setLegendWidth(int legendWidth);
	void setTextSpacing(int legendPadding, int textSpacing);
	
	void setYScale(float yScale);
	float getYScale();
	void setYOffset(float yOffset);
	float getYOffset();
	
	string getVariableName(int i);
};



/*-------------------------------------------------
* ofxMultiScope
* Class to handle the plotting of data on multiple oscilloscope panels
* grouped together in one larger panel.
*-------------------------------------------------*/
class ofxMultiScope {
private:
	ofPoint _min;
	ofPoint _max;
	int _numScopes;
public:
	std::vector<ofxOscilloscope> scopes;

	// Constructors
	ofxMultiScope();
	ofxMultiScope(int numScopes, ofRectangle plotArea=ofRectangle(ofPoint(0,0), ofGetWindowSize()), 
		ofTrueTypeFont legendFont=ofTrueTypeFont(), int legendWidth=100,
		ofColor outlineColor=ofColor(200,200,200), ofColor zeroLineColor=ofColor(240,240,240),
		ofColor backgroundColor=ofColor(0.,0.,0.,0.));
	ofxMultiScope(int numScopes, ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(), 
		ofTrueTypeFont legendFont=ofTrueTypeFont(), int legendWidth=100,
		ofColor outlineColor=ofColor(200,200,200), ofColor zeroLineColor=ofColor(240,240,240),
		ofColor backgroundColor=ofColor(0.,0.,0.,0.));
	// Destructor
	~ofxMultiScope();

	// Plotting
	void plot();

	// Setters/Getters
	void setTimeWindow(float timeWindow);
	void setPosition(ofPoint min, ofPoint max);
	void setPosition(ofRectangle scopeArea);
	ofRectangle getPosition();

	void setOutlineColor(ofColor outlineColor);
	void setZeroLineColor(ofColor zeroLineColor);
	void setBackgroundColor(ofColor backgroundColor);

	void setLegendFont(ofTrueTypeFont legendFont);
	void setLegendWidth(int legendWidth);
	void setTextSpacing(int legendPadding, int textSpacing);

};

#endif
