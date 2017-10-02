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
	float _plotLineWidth;
public:
	// Constructors
	ofxScopePlot(ofRectangle plotArea,
		ofColor zeroLineColor=ofColor(240,240,240), ofColor backgroundColor=ofColor(0.,0.,0.,0.),
		float plotLineWidth=1.);
	ofxScopePlot(ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(),
		ofColor zeroLineColor=ofColor(240,240,240), ofColor backgroundColor=ofColor(0.,0.,0.,0.),
		float plotLineWidth=1.);
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

	void setPlotLineWidth(float plotLineWidth);

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
	float _outlineWidth;
	ofColor _backgroundColor;

public:
	// Constructors
	ofxOscilloscope(ofRectangle scopeArea, 
		ofTrueTypeFont legendFont=ofTrueTypeFont(), int legendWidth=100,
		ofColor outlineColor=ofColor(200,200,200), ofColor zeroLineColor=ofColor(240,240,240),
		ofColor backgroundColor=ofColor(0.,0.,0.,0.),
		float plotLineWidth=1., float outlineWidth=1.);
	ofxOscilloscope(ofPoint min=ofPoint(0,0), ofPoint max=ofGetWindowSize(), 
		ofTrueTypeFont legendFont=ofTrueTypeFont(), int legendWidth=100,
		ofColor outlineColor=ofColor(200,200,200), ofColor zeroLineColor=ofColor(240,240,240),
		ofColor backgroundColor=ofColor(0.,0.,0.,0.),
		float plotLineWidth=1., float outlineWidth=1.);
	// Destructor
	~ofxOscilloscope();

	// Setup of scope
	void setup(float timeWindow, float sampFreq,
		string variableNames, ofColor variableColors,
		float yScale = 1.0, float yOffset = 0.0);
	void setup(float timeWindow, float sampFreq,
		std::vector<string> variableNames, std::vector<ofColor> variableColors,
		float yScale = 1.0, float yOffset = 0.0);
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
	void setVariableNames(std::vector<string> variableNames);		// Variable names
	void setVariableNames(string variableNames[], int nVariables);	// Variable names

	void setVariableColors(std::vector<ofColor> colors);			// Variable colors
	void setVariableColors(ofColor colors[], int nColors);			// Variable colors

	void setTimeWindow(float timeWindow);		// Duration of displayed data window (seconds)
	float getTimeWindow();						// Duration of displayed data window (seconds)
	void setPosition(ofPoint min, ofPoint max);	// Position of the scope panel
	void setPosition(ofRectangle scopeArea);	// Position of the scope panel
	ofRectangle getPosition();					// Position of the scope panel
	//void setPosition(ofPoint center, int height, int width);

	void setPlotLineWidth(float plotLineWidth);			// Line width of plot data
	void setOutlineWidth(float outlineWidth);			// Line width of scope outlines
	void setOutlineColor(ofColor outlineColor);			// Line color of scope outlines
	void setZeroLineColor(ofColor zeroLineColor);		// Line color of scope zero lines
	void setBackgroundColor(ofColor backgroundColor);	// Background color of scopes

	void setLegendFont(ofTrueTypeFont legendFont);				// Legend font
	void setLegendWidth(int legendWidth);						// Width of legend
	void setTextSpacing(int legendPadding, int textSpacing);	// Vertical spacing of legend text

	void setYScale(float yScale);	// yScale of plotted data
	float getYScale();				// yScale of plotted data
	void setYOffset(float yOffset);	// yScale of plotted data
	float getYOffset();				// yScale of plotted data

	float incrementYScale();		// Change yScale of plotted data
	float decrementYScale();		// Change yScale of plotted data
	float incrementYOffset();		// Change y“ffset of plotted data
	float decrementYOffset();		// Change y“ffset of plotted data
	float incrementTimeWindow();	// Change duration of displayed data window
	float decrementTimeWindow();	// Change duration of displayed data window

	string getVariableName(int i);	// Name of a selected variable

	static float round(float number) {	// General purpose function that should be built in
		return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
	}
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
	void setTimeWindow(float timeWindow);		// Duration of displayed data window (seconds)
	void setPosition(ofPoint min, ofPoint max);	// Position of the multiScope panel
	void setPosition(ofRectangle scopeArea);	// Position of the multiScope panel
	ofRectangle getPosition();					// Position of the multiScope panel

	void setPlotLineWidth(float plotLineWidth);			// Line width of plot data
	void setOutlineWidth(float outlineWidth);			// Line width of scope outlines
	void setOutlineColor(ofColor outlineColor);			// Line color of scope outlines
	void setZeroLineColor(ofColor zeroLineColor);		// Line color of scope zero lines
	void setBackgroundColor(ofColor backgroundColor);	// Background color of scopes

	void setLegendFont(ofTrueTypeFont legendFont);				// Legend font
	void setLegendWidth(int legendWidth);						// Width of legend
	void setTextSpacing(int legendPadding, int textSpacing);	// Vertical spacing of legend text

	vector<float> incrementYScale();		// Change yScale of plotted data
	vector<float> decrementYScale();		// Change yScale of plotted data
	vector<float> incrementYOffset();		// Change y“ffset of plotted data
	vector<float> decrementYOffset();		// Change y“ffset of plotted data
	vector<float> incrementTimeWindow();	// Change duration of displayed data window
	vector<float> decrementTimeWindow();	// Change duration of displayed data window

};

#endif
