//
//  ofxOscilloscope.cpp
//
//  openFrameworks addOn to display data as on an oscilloscope
//
//  Created by Sean Montgomery on 12/21/12.
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//
#include "ofxOscilloscope.h"

/*-------------------------------------------------
* ofxScopePlot
* Class to handle the plotting of data for ofxOscilloscope
*-------------------------------------------------*/

/*
** ofxScopePlot
*/
ofxScopePlot::ofxScopePlot(ofRectangle plotArea, ofColor zeroLineColor, 
	ofColor backgroundColor, float plotLineWidth) {
		setPosition(plotArea);
		setZeroLineColor(zeroLineColor);
		setBackgroundColor(backgroundColor);
		setPlotLineWidth(plotLineWidth);
		_yLims = pair<float, float>(0.f, 0.f);
}

/*
** ofxScopePlot
*/
ofxScopePlot::ofxScopePlot(ofPoint min, ofPoint max, ofColor zeroLineColor,
	ofColor backgroundColor, float plotLineWidth) {
		setPosition(min, max);
		setZeroLineColor(zeroLineColor);
		setBackgroundColor(backgroundColor);
		setPlotLineWidth(plotLineWidth);
		_yLims = pair<float, float>(0.f, 0.f);
}

/*
** ~ofxScopePlot
*/
ofxScopePlot::~ofxScopePlot() {
}

/*
** setup
** Initializes the ScopePlot. 
** 
** INPUT:
** timeWindow		Specifies the time in seconds of the oscilloscope window
** sampFreq			Specifies the sampling frequency of incoming data
** variableColors	Display colors of variables
** yScale			Muliplier for Y scaling axis
** yOffset			Offset for the Y axis
*/
void ofxScopePlot::setup(float timeWindow, float sampFreq, std::vector<ofColor> variableColors, 
	float yScale, float yOffset) {
		_timeWindow = timeWindow;
		_pointsPerWin = floor(_timeWindow * sampFreq); //(sec)
		_sampFreq = sampFreq;
		_nVariables = variableColors.size();
		_buffer.clear();
		for (int i=0; i<_nVariables; i++) {
			_buffer.push_back(std::vector<float>(_pointsPerWin, 0.));
		}

		setYScale(yScale);
		setYOffset(yOffset);

		setVariableColors(variableColors);
}

/*
** setup
** Initializes the ScopePlot. 
** 
** INPUT:
** timeWindow		Specifies the time in seconds of the oscilloscope window
** sampFreq			Specifies the sampling frequency of incoming data
** variableColors	Display colors of variables
** nVariables		number of variables
** yScale			Muliplier for Y scaling axis
** yOffset			Offset for the Y axis
*/
void ofxScopePlot::setup(float timeWindow, float sampFreq, ofColor variableColors[], int nVariables, 
		float yScale, float yOffset) {

			std::vector<ofColor> vec_variableColors;
			vec_variableColors.assign(variableColors, variableColors + nVariables);

			setup(timeWindow, sampFreq, vec_variableColors, yScale, yOffset);
}


/*
** updateData
**
** Data should come in the form[nVariables] with one data point/variable
**
** Loads new data onto the ScopePlot buffer. Must have the same number
** of variables initialized with setup. plot() may be called to display the 
** updated buffer.
*/
void ofxScopePlot::updateData(std::vector<float> data) {
#ifdef DEBUG_PRINT
	printf("ofxScopePlot::updateData\n"); 
#endif
	if (data.size() != getNumVariables()) {
		fprintf(stderr, "ERROR: data.size() != getNumVariables()");
	} else {
		for (int i=0; i<data.size(); i++) {

			// Insert the data elements into the beginning of _buffer
			_buffer.at(i).insert(_buffer.at(i).begin(), data.at(i));

			// Erase elements at end of _buffer
			_buffer.at(i).erase(_buffer.at(i).begin() + _pointsPerWin, _buffer.at(i).end());

		}
	}
}


/*
** updateData
**
** Data should come in the form[nVariables][nDataPoints]
**
** Loads new data onto the ScopePlot buffer. Must have the same number
** of variables initialized with setup. plot() may be called to display the 
** updated buffer.
*/
void ofxScopePlot::updateData(std::vector<std::vector<float> > data) {
#ifdef DEBUG_PRINT
	printf("ofxScopePlot::updateData\n"); 
#endif
	if (data.size() != getNumVariables()) {
		fprintf(stderr, "ERROR: data.size() != getNumVariables()");
	} else {
		for (int i=0; i<data.size(); i++) {
			//int nPoints = data.at(i).size();
			//int buffPoints = _buffer.at(i).size();

			// Reverse the elements of incoming data
			reverse(data.at(i).begin(), data.at(i).end());

			// Insert the data elements into the beginning of _buffer
			_buffer.at(i).insert(_buffer.at(i).begin(), data.at(i).begin(), data.at(i).end());

			// Erase elements at end of _buffer
			_buffer.at(i).erase(_buffer.at(i).begin() + _pointsPerWin, _buffer.at(i).end());

			int junk;
		}
	}
}

/*
** updateData
**
** UNSAFE: Use updateData(std::vector<std::vector<float>> data)
** 
** Data should come in the form[nVariables][nDataPoints]
**
** Loads new data onto the ScopePlot buffer. Must have the same number
** of variables initialized with setup. plot() may be called to display the 
** updated buffer.
*/
void ofxScopePlot::updateData(float ** data, int nPoints) {

	std::vector< std::vector<float> > vec_data;

	vec_data.resize(getNumVariables());
	for (int i=0; i<getNumVariables(); i++) {
		vec_data.at(i).resize(nPoints);
		for (int j=0; j<nPoints; j++) {
			vec_data.at(i).at(j) = data[i][j];
		}
		//vec_data.at(i).assign(data[i], data[i] + nPoints
	}
	updateData(vec_data);
}

void ofxScopePlot::clearData() {
	for (int i = 0; i < _buffer.size(); i++) {
		if (_buffer.at(i).size() > 0) {
			float lastVal = _buffer.at(i).at(0);
			_buffer.at(i).assign(_buffer.at(i).size(), lastVal);
		}
	}
}


/*
** setVariableColors
** Sets the plot colors of the variables.
** Must be called after setup with the same number of variables initialized.
*/
void ofxScopePlot::setVariableColors(std::vector<ofColor> colors) {
	if (colors.size() != getNumVariables()) {
		fprintf(stderr, "ERROR: colors.size() != getNumVariables()");
	} else {
		_variableColors = colors;
	}
}

/*
** setVariableColors
** Sets the plot colors of the variables.
** Must be called after setup with the same number of variables initialized.
*/
void ofxScopePlot::setVariableColors(ofColor colors[], int nColors) {

	std::vector<ofColor> vec_colors;
	vec_colors.assign(colors, colors + nColors);

	setVariableColors(vec_colors);
}

/*
** setPlotLineWidth
** Sets line width of the data plot
*/
void ofxScopePlot::setPlotLineWidth(float plotLineWidth) {
	_plotLineWidth = plotLineWidth;
}

/*
** setZeroLineColor
** Sets the color of the zero line.
*/
void ofxScopePlot::setZeroLineColor(ofColor zeroLineColor) {
	_zeroLineColor = zeroLineColor;
}

/*
** setBackgroundColor
** Sets the color of the background.
*/
void ofxScopePlot::setBackgroundColor(ofColor backgroundColor) {
	_backgroundColor = backgroundColor;
}

/*
** setPosition
** Sets the position of the oscilloscope window.
*/ 
void  ofxScopePlot::setPosition(ofPoint min, ofPoint max) {
	_min = min;
	_max = max;
}
void  ofxScopePlot::setPosition(ofRectangle plotArea) {
	setPosition(plotArea.getTopLeft(), plotArea.getBottomRight());
}

/*
** getPosition
** gets the rectangle defining the position of the oscilloscope window.
*/ 
ofRectangle ofxScopePlot::getPosition() {
	ofRectangle rect = ofRectangle((const ofPoint) _min, (const ofPoint) _max);
	return rect;
}

/*
** setTimeWindow
** Sets the timeWindow covered by the scope.
*/ 
void ofxScopePlot::setTimeWindow(float timeWindow) {
	_timeWindow = timeWindow;
	_pointsPerWin = floor(_timeWindow * _sampFreq); //(sec)
	for (int i=0; i<_nVariables; i++) {
		_buffer.at(i).resize(_pointsPerWin);
	}
}

/*
** setYScale
** Sets the yScale of the data in the oscilloscope window.
*/ 
void ofxScopePlot::setYScale(float yScale) {
	_yScale = yScale;
}

/*
** getYScale
** Gets the yScale of the data in the oscilloscope window.
*/ 
float ofxScopePlot::getYScale() {
	return _yScale;
}

/*
** setYOffset
** Sets the yOffset of the data in the oscilloscope window.
*/ 
void ofxScopePlot::setYOffset(float yOffset) {
	_yOffset = yOffset;
}

/*
** getYOffset
** Gets the yOffset of the data in the oscilloscope window.
*/ 
float ofxScopePlot::getYOffset() {
	return _yOffset;
}

/*
** getMinMaxY
** Returns the min and max values of the data in the oscilloscope window as an std::pair
*/
pair<float, float> ofxScopePlot::getMinMaxY() {
	vector<float> minMaxY;
	for (int j = 0; j < _buffer.size(); j++) {
		// calculate the min and max for each plot in the scope 
		auto result = std::minmax_element(_buffer.at(j).begin(), _buffer.at(j).end());
		minMaxY.push_back(*result.first);
		minMaxY.push_back(*result.second);
	}
	// return the min and max across all scope plots
	auto result = std::minmax_element(minMaxY.begin(), minMaxY.end());
	return std::pair<float, float>(*(result.first), *(result.second));
}

/*
** setMinMaxY
** Sets the min and max values of the data in the oscilloscope window as an std::pair
*/
void ofxScopePlot::setMinMaxY(pair<float, float> yLims) {
	_yLims = yLims;
	float plotHeight = ofGetWindowSize().y;
	float ySpan = yLims.second - yLims.first;
	setYScale(plotHeight / ySpan);
	float yOffset = (yLims.second + yLims.first) / 2.f;
	setYOffset(-yOffset * plotHeight / ySpan);
}

pair<float, float> ofxScopePlot::getYLims() {
	return _yLims;
}

/*
** plot
** Plots the data in the buffer
*/
void ofxScopePlot::plot() {
	// ToDo:: simplify code to use ofPushMatrix()

	//float xPlotScale = ofGetWindowSize().x / _pointsPerWin * ofGetWindowSize().x / (_max.x - _min.x);
	float xPlotScale = (_max.x - _min.x) / (_pointsPerWin - 1);// * (_max.x - _min.x) / ofGetWindowSize().x;
	float yPlotScale = (_max.y - _min.y) / ofGetWindowSize().y;
	float yPlotOffset = ((_max.y - _min.y) / 2.);
	//printf("xPlotScale: %f, yPlotScale: %f, yPlotOffset: %f\n", xPlotScale, yPlotScale, yPlotOffset);
	//ofScale(1., 1., 1.);

	// Background
	ofEnableAlphaBlending();
	ofSetColor(_backgroundColor);
	ofRect(ofRectangle(_min, _max));

	// Scope zero line
	ofSetColor(_zeroLineColor);
	ofLine(_min.x, _min.y + (_max.y - _min.y)/2, _max.x, _min.y + (_max.y - _min.y)/2);
	ofDisableAlphaBlending(); 

	for (int i=0; i<getNumVariables(); i++) {
		ofSetColor(_variableColors.at(i));
		ofSetLineWidth(_plotLineWidth);
		for (int j=1; j<_pointsPerWin; j++) {
			ofPoint p1 = ofPoint(_max.x-((float)(j-1)*xPlotScale),  
				_max.y-(yPlotScale*((_buffer.at(i).at(j-1) * _yScale + _yOffset)) + yPlotOffset));

			ofPoint p2 = ofPoint(_max.x-((float)(j)*xPlotScale), 
				_max.y-(yPlotScale*((_buffer.at(i).at(j) * _yScale + _yOffset)) + yPlotOffset));

			ofLine(p1, p2);
			//printf("[%i, %i]: ofLine([%.1f, %.1f], [%.1f, %.1f]): buffer[%.1f],[%.1f]\n", i, j, p1.x, p1.y, p2.x, p2.y, _buffer[i][j-1], _buffer[i][j]);
			//ofLine(_max.x-(float)(j-1), (_buffer[i][j-1] * _yScale) + yPlotOffset, _max.x-(float)j, (_buffer[i][j] * _yScale) + yPlotOffset);
			//printf("[%i, %i]: ofLine(%f, %f, %f, %f)\n", i, j, _max.x-(float)(j-1), (_buffer[i][j-1] * _yScale) + yPlotOffset, _max.x-(float)j, (_buffer[i][j] * _yScale) + yPlotOffset);
		}
	}
}

/*
** getNumVariables
** Returns the number of variables in the current data buffer
*/
int ofxScopePlot::getNumVariables() {
	return _buffer.size();
	//return _nVariables;
}

/*
** getVariableColor
*/
ofColor ofxScopePlot::getVariableColor(int i) {
	if (i < _variableColors.size()) {
		return _variableColors.at(i);
	} else {
		fprintf(stderr, "ERROR - getVariableColor: uninitialized or invalid index");
		return NULL;
	}
}

/*
** getTimeWindow
** Returns the length of the current scope plot window in seconds.
*/
float ofxScopePlot::getTimeWindow() {
	return _timeWindow;
}

/*
** getSamplingFrequency
** Returns the sampling frequency in Hz
*/
float ofxScopePlot::getSamplingFrequency() {
	return _sampFreq;
}


/*-------------------------------------------------
* ofxOscilloscope
* Class to create an oscilloscope panel
*-------------------------------------------------*/
/*
ofxOscilloscope::ofxOscilloscope() {
	_variableNames = NULL;
	//ofTrueTypeFont::setGlobalDpi(72);

	_legendFont.loadFont("verdana.ttf", 12, true, true);
	//_legendFont.setLineHeight(18.0f);
	//_legendFont.setLetterSpacing(1.037);

	_min=ofPoint(0,0);
	_max=ofGetWindowSize();
	_legendWidth=100;
}
*/
ofxOscilloscope::ofxOscilloscope(ofRectangle scopeArea, ofTrueTypeFont legendFont,
	int legendWidth, ofColor outlineColor, ofColor zeroLineColor, ofColor backgroundColor,
	float plotLineWidth, float outlineWidth) {
	_min = scopeArea.getTopLeft();
	_max = scopeArea.getBottomRight();
	_legendWidth = legendWidth;

	// Create scopePlot
	ofPoint min = _min;
	ofPoint max = _max;
	min.x = min.x + _legendWidth;
	_scopePlot = ofxScopePlot(min, max, zeroLineColor, plotLineWidth);

	setBackgroundColor(backgroundColor);
	setOutlineColor(outlineColor);
	setOutlineWidth(outlineWidth);

	if(legendFont.isLoaded()) {
		setLegendFont(legendFont);
		setAxesFont(legendFont);
	} else {
		//_legendFont.loadFont("verdana.ttf", 12, true, true);
			//_legendFont.setLineHeight(18.0f);
		//_legendFont.setLetterSpacing(1.037);
		//ofTrueTypeFont::setGlobalDpi(72);
	}

	// Default values
	_legendPadding = 10;
	_textSpacer = 20;
	_autoscaleY = false;
	_minYSpan = 0.f;
}

ofxOscilloscope::ofxOscilloscope(ofPoint min, ofPoint max, ofTrueTypeFont legendFont,
	int legendWidth, ofColor outlineColor, ofColor zeroLineColor, ofColor backgroundColor,
	float plotLineWidth, float outlineWidth) {
	_min = min;
	_max = max;
	_legendWidth = legendWidth;

	// Create scopePlot
	min.x = min.x + _legendWidth;
	_scopePlot = ofxScopePlot(min, max, zeroLineColor, plotLineWidth);


	setBackgroundColor(backgroundColor);
	setOutlineColor(outlineColor);
	setOutlineWidth(outlineWidth);

	if(legendFont.isLoaded()) {
		setLegendFont(legendFont);
		setAxesFont(legendFont);
	} else {
		//_legendFont.loadFont("verdana.ttf", 12, true, true);
			//_legendFont.setLineHeight(18.0f);
		//_legendFont.setLetterSpacing(1.037);
		//ofTrueTypeFont::setGlobalDpi(72);
	}

	// Default values
	_legendPadding = 10;
	_textSpacer = 20;
	_autoscaleY = false;
	_minYSpan = 0.f;
}

/*
** ~ofxOscilloscope
*/ 
ofxOscilloscope::~ofxOscilloscope() {
}

/*
** setup
** Initializes the oscilloscope. 
** 
** INPUT:
** timeWindow		Specifies the time in seconds of the oscilloscope window
** sampFreq			Specifies the sampling frequency of incoming data
** variableNames	Display names for variables
** variableColors	Display colors of variables
** yScale			Muliplier for Y scaling axis
** yOffset			Offset for the Y axis
**
** REMARKS:
** variableNames and variableColors must be the same size.
*/
void ofxOscilloscope::setup(float timeWindow, float sampFreq,
	string variableName, ofColor variableColor,
	float yScale, float yOffset) {

	vector<string> variableNames;
	variableNames.push_back(variableName);

	vector<ofColor> variableColors;
	variableColors.push_back(variableColor);

	setup(timeWindow, sampFreq,
		variableNames, variableColors,
		yScale, yOffset);
}


void ofxOscilloscope::setup(float timeWindow, float sampFreq,
	std::vector<string> variableNames, std::vector<ofColor> variableColors,
	float yScale, float yOffset) {

	//if (variableNames.size() > variableColors.size()) {
	//	fprintf(stderr, "ERROR: variableNames.size() > variableColors.size()");
	if (variableNames.size() > variableColors.size()) {
		for (int i = variableNames.size(); i < variableColors.size(); i++) {
			variableColors.emplace_back(0, 0, 0);
		}
	}
	if (variableNames.size() < variableColors.size()) {
		vector<ofColor> temp = vector<ofColor>(variableColors.begin(), variableColors.begin() + variableNames.size());
		variableColors = temp;
	}

	//} else {
	ofPoint min = _min;
	min.x = min.x + _legendWidth;

	_scopePlot.setup(timeWindow, sampFreq, variableColors, yScale, yOffset);
	setVariableNames(variableNames);
}

/*
** setup
** Initializes the oscilloscope. 
** 
** INPUT:
** timeWindow		Specifies the time in seconds of the oscilloscope window
** sampFreq			Specifies the sampling frequency of incoming data
** variableNames	Display names for variables
** variableColors	Display colors of variables
** nVariables		Number of variables to plot
** yScale			Muliplier for Y scaling axis
** yOffset			Offset for the Y axis
**
** REMARKS:
** variableNames and variableColors must be the same size.
*/
void ofxOscilloscope::setup(float timeWindow, float sampFreq, string variableNames[], 
	ofColor variableColors[], int nVariables, float yScale, float yOffset) {

		std::vector<string> vec_variableNames;
		vec_variableNames.assign(variableNames, variableNames + nVariables);

		std::vector<ofColor> vec_variableColors;
		vec_variableColors.assign(variableColors, variableColors + nVariables);

		setup(timeWindow, sampFreq, vec_variableNames, vec_variableColors, yScale, yOffset);
}

/*
** setVariableNames
** Must be called after setup with the same number of variables initialized.
*/
void ofxOscilloscope::setVariableNames(std::vector<string> variableNames) {
	if (variableNames.size() != _scopePlot.getNumVariables()) {
		fprintf(stderr, "ERROR: variableNames.size() != getNumVariables()");
	} else {
		_variableNames = variableNames;
	}
}

/*
** setVariableNames
** Must be called after setup with the same number of variables initialized.
*/
void ofxOscilloscope::setVariableNames(string variableNames[], int nVariables) {
	std::vector<string> vec_variableNames;
	vec_variableNames.assign(variableNames, variableNames + nVariables);

	setVariableNames(vec_variableNames);
}

/*
** setVariableColors
** Must be called after setup with the same number of variables initialized.
*/
void ofxOscilloscope::setVariableColors(std::vector<ofColor> colors){
	_scopePlot.setVariableColors(colors);
}

/*
** setVariableColors
** Must be called after setup with the same number of variables initialized.
*/
void ofxOscilloscope::setVariableColors(ofColor colors[], int nColors) {
		std::vector<ofColor> vec_colors;
		vec_colors.assign(colors, colors + nColors);

		setVariableColors(vec_colors);
}

/*
** setPlotLineWidth
** Sets the width of the scope outlines
*/
void ofxOscilloscope::setPlotLineWidth(float plotLineWidth) {
	_scopePlot.setPlotLineWidth(plotLineWidth);
}

/*
** setOutlineWidth
** Sets the width of the scope outlines
*/
void ofxOscilloscope::setOutlineWidth(float outlineWidth) {
	_outlineWidth = outlineWidth;
}

/*
** setOutlineColor
** Sets the color of the scope outlines
*/
void ofxOscilloscope::setOutlineColor(ofColor outlineColor) {
	_outlineColor = outlineColor;
}

/*
** setZeroLineColor
** Sets the color of the scope zero line
*/
void ofxOscilloscope::setZeroLineColor(ofColor zeroLineColor) {
	_scopePlot.setZeroLineColor(zeroLineColor);
}

/*
** setBackgroundColor
** Sets the color of the background.
*/
void ofxOscilloscope::setBackgroundColor(ofColor backgroundColor) {
	_backgroundColor = backgroundColor;
	_scopePlot.setBackgroundColor(backgroundColor);
}

/*
** setLegendFont
** Sets the font used for the legend text
*/ 
void ofxOscilloscope::setLegendFont(ofTrueTypeFont legendFont) {
	_legendFont = legendFont;
}

/*
** setAxesFont
** Sets the font used for the axes text
*/
void ofxOscilloscope::setAxesFont(ofTrueTypeFont axesFont) {
	_axesFont = axesFont;
}

/*
** setLegendWidth
** Sets the width of the legend
*/ 
void ofxOscilloscope::setLegendWidth(int legendWidth) {
	_legendWidth = legendWidth;
	setPosition(getPosition());
}

/*
** setTextSpacing
** Sets the spacing of text in the legend.
** legendPadding is the padding between the legend borders and the text.
** textSpacing the spacing between the text of the variable names.
*/ 
void ofxOscilloscope::setTextSpacing(int legendPadding, int textSpacing) {
	_legendPadding = legendPadding;
	_textSpacer = textSpacing;
}

/*
** setPosition
** Sets the position of the oscilloscope window.
*/ 
void ofxOscilloscope::setPosition(ofPoint min, ofPoint max) {
	_min = min;
	_max = max;

	// reposition the scopePlot
	min.x = min.x + _legendWidth;
	_scopePlot.setPosition(min, max);
}
void  ofxOscilloscope::setPosition(ofRectangle scopeArea) {
	setPosition(scopeArea.getTopLeft(), scopeArea.getBottomRight());
}

/*
** getPosition
** gets the rectangle defining the position of the oscilloscope window.
*/ 
ofRectangle ofxOscilloscope::getPosition() {
	ofRectangle rect = ofRectangle((const ofPoint) _min, (const ofPoint) _max);
	return rect;
}

/*
** getSamplingFreq
** Sets the sampling frequency
*/
float ofxOscilloscope::getSamplingFrequency() {
	return _scopePlot.getSamplingFrequency();
}

/*
** setTimeWindow
** Sets the timeWindow covered by the scope.
*/ 
void ofxOscilloscope::setTimeWindow(float timeWindow) {
	_scopePlot.setTimeWindow(timeWindow);
}

/*
** getTimeWindow
** gets the timeWindow covered by the scope.
*/ 
float ofxOscilloscope::getTimeWindow() {
	return _scopePlot.getTimeWindow();
}

/*
** setYScale
** Sets the yScale of the data in the oscilloscope window.
*/ 
void ofxOscilloscope::setYScale(float yScale) {
	_scopePlot.setYScale(yScale);
}

/*
** getYScale
** Gets the yScale of the data in the oscilloscope window.
*/ 
float ofxOscilloscope::getYScale() {
	return _scopePlot.getYScale();
}

/*
** setYOffset
** Sets the yOffset of the data in the oscilloscope window.
*/ 
void ofxOscilloscope::setYOffset(float yOffset) {
	_scopePlot.setYOffset(yOffset);
}

/*
** getYOffset
** Gets the yOffset of the data in the oscilloscope window.
*/ 
float ofxOscilloscope::getYOffset() {
	return _scopePlot.getYOffset();
}

/*
** autoscaleY
** sets the autoscaling of the oscilloscope Y-axis
*/
void ofxOscilloscope::autoscaleY(bool autoscale, float minYSpan) {
	_autoscaleY = autoscale;
	_minYSpan = minYSpan;
}

float ofxOscilloscope::getMinYSpan() {
	return _minYSpan;
}

void ofxOscilloscope::setYLims(pair<float, float> yLims) {
	_scopePlot.setMinMaxY(yLims);
}

pair<float, float> ofxOscilloscope::getYLims() {
	return _scopePlot.getYLims();
}

/*
** incrementYScale
** Increases the yScale to yScale*2
*/ 
float ofxOscilloscope::incrementYScale() {
	setYScale(getYScale() * 2);
	return getYScale();
}

/*
** decrementYScale
** Decreases the yScale to yScale/2
*/ 
float ofxOscilloscope::decrementYScale() {
	setYScale(getYScale() / 2);
	return getYScale();
}

/*
** incrementYOffset
** Increases the yOffset on an eponential scale
*/ 
float ofxOscilloscope::incrementYOffset() {
	float inc = 2.71828/2;

	if (getYOffset() == 0) { 
		setYOffset(1);
	} else if (getYOffset() == -1) { 
		setYOffset(0);
	} else if (getYOffset() > 0) {
		setYOffset(pow(inc, round(log(getYOffset())/log(inc)) + 1));
	} else if (getYOffset() < 0) {
		setYOffset(-pow(inc, round(log(-getYOffset())/log(inc)) - 1));
	}
	return getYOffset();
}

/*
** decrementYOffset
** Decreases the yOffset on an eponential scale
*/ 
float ofxOscilloscope::decrementYOffset() {
	float inc = 2.71828/2;

	if (getYOffset() == 0) { 
		setYOffset(-1);
	} else if (getYOffset() == 1) { 
		setYOffset(0);
	} else if (getYOffset() > 0) {
		setYOffset(pow(inc, round(log(getYOffset())/log(inc)) - 1));
	} else if (getYOffset() < 0) {
		setYOffset(-pow(inc, round(log(-getYOffset())/log(inc)) + 1));
	}
	return getYOffset();
}

/*
** incrementTimeWindow
** Increases the timeWindow to timeWindow*2
*/ 
float ofxOscilloscope::incrementTimeWindow() {
	setTimeWindow(getTimeWindow() * 2);
	return getTimeWindow();
}

/*
** decrementTimeWindow
** Decreases the timeWindow to timeWindow/2
*/ 
float ofxOscilloscope::decrementTimeWindow() {
	setTimeWindow(getTimeWindow() / 2);
	return getTimeWindow();
}

/*
** updateData
**
** Data should come in the form data[nVariables] with one data point/variable
** or in form data[nDataPoints] iff scope is setup with variableNames.size() == 1
**
** Loads new data onto the oscillocope buffer. Must have the same number
** of variables initialized with setup. plot() may be called to display the 
** updated buffer.
*/
void ofxOscilloscope::updateData(std::vector<float> data) {
#ifdef DEBUG_PRINT
	printf("ofxOscilloscope::updateData\n"); 
#endif
	if (_variableNames.size() > 1)
	{
		_scopePlot.updateData(data);
	}
	else
	{
		// Reshape data to easily handle case where we're plotting a single data stream
		vector<vector<float>> temp;
		temp.resize(1);
		temp.at(0) = data;
		_scopePlot.updateData(temp);
	}
}

/*
** updateData
**
** Data should come in the form[nVariables][nDataPoints]
**
** Loads new data onto the oscillocope buffer. Must have the same number
** of variables initialized with setup. plot() may be called to display the 
** updated buffer.
*/
void ofxOscilloscope::updateData(std::vector<std::vector<float> > data) {
#ifdef DEBUG_PRINT
	printf("ofxOscilloscope::updateData\n"); 
#endif
	_scopePlot.updateData(data);
}


/*
** updateData
**
** Data should come in the form [nDataPoints]
**
** Loads new data onto the oscillocope buffer. Must have the same number
** of variables initialized with setup. plot() may be called to display the
** updated buffer.
*/
void ofxOscilloscope::updateData(size_t variableNum, std::vector<float> data) {
#ifdef DEBUG_PRINT
	printf("ofxOscilloscope::updateData\n");
#endif
	size_t numVars = _scopePlot.getNumVariables();
	if (variableNum < numVars)
	{
		std::vector<std::vector<float>> multiData;
		multiData.resize(numVars);
		multiData.at(variableNum) = data;
		_scopePlot.updateData(multiData);
	}
}


/*
** updateData (float ** data, int nPoints)
**
** UNSAFE: Use updateData(std::vector<std::vector<float>> data)
** 
** Data should come in the form[nVariables][nDataPoints]
**
** Loads new data onto the oscillocope buffer. Must have the same number
** of variables initialized with setup. plot() may be called to display the 
** updated buffer.
*/
void ofxOscilloscope::updateData(float ** data, int nPoints) {
#ifdef DEBUG_PRINT
	printf("ofxOscilloscope::updateData\n"); 
#endif
	_scopePlot.updateData(data, nPoints);
}

void ofxOscilloscope::clearData() {
	_scopePlot.clearData();
}

/*
** plot
** Plots the data in the buffer
*/
void ofxOscilloscope::plot(){

	ofPushStyle();

	// Legend Background
	ofEnableAlphaBlending();
	ofSetColor(_backgroundColor);
	ofPoint legendMax = ofPoint(_min.x + _legendWidth, _max.y);
	ofRect(ofRectangle(_min, legendMax));
	ofDisableAlphaBlending(); 

	ofSetColor(_outlineColor);

	if (_autoscaleY) {
		auto minMaxY = _scopePlot.getMinMaxY();
		float ySpan = minMaxY.second - minMaxY.first;
		float meanXY = (minMaxY.first + minMaxY.second) / 2;
		if (_minYSpan > ySpan) {
			minMaxY.second = meanXY + _minYSpan / 2;
			minMaxY.first = meanXY - _minYSpan / 2;
		}
		ySpan = minMaxY.second - minMaxY.first;
		meanXY = (minMaxY.first + minMaxY.second) / 2;

		// Using ofGetWindowHeight() here is messy
		// ToDo: Consider refactoring code to make use ofScale() and ofTranslate() instead multiplication and subtraction
		float yScale = ofGetWindowHeight() / (ySpan);
		float yOffset = - meanXY * yScale;
		//float yOffset = 0;
		_scopePlot.setYOffset(yOffset);
		_scopePlot.setYScale(yScale);
	}

	// Timescale
	string axesString = ofToString(_scopePlot.getTimeWindow()) + " sec," + " yScale=" + ofToString(getYScale())
		+ ", yOffset=" + ofToString(getYOffset(), 1);
	float legendX = _min.x + _legendWidth + _legendPadding;
	float legendY = _max.y - _legendPadding;
	float yLabelPadding = 3;
	float yValX = _min.x + _legendWidth - yLabelPadding;
	float yValY = (_min.y + _max.y) / 2;

	if (_axesFont.isLoaded())
	{
		_axesFont.drawString(axesString, legendX, legendY);
	}
	else
	{
		ofDrawBitmapString(axesString, legendX, legendY);
	}

	
	string yVal;
	ofRectangle yValBox; 

	if (_axesFont.isLoaded()) {

		yVal = ofToString(-getYOffset() / getYScale());
		yValBox = _axesFont.getStringBoundingBox(yVal, 0, 0);
		_axesFont.drawString(yVal, yValX - yValBox.getRight(), yValY + _axesFont.getAscenderHeight() / 2);

		yVal = ofToString((-getYOffset() + ofGetWindowHeight() / 2) / getYScale());
		yValBox = _axesFont.getStringBoundingBox(yVal, 0, 0);
		_axesFont.drawString(yVal, yValX - yValBox.getRight(), _min.y - yValBox.getTop() + yLabelPadding);

		yVal = ofToString((-getYOffset() - ofGetWindowHeight() / 2) / getYScale());
		yValBox = _axesFont.getStringBoundingBox(yVal, 0, 0);
		_axesFont.drawString(yVal, yValX - yValBox.getRight(), _max.y - yLabelPadding);

	}
	else
	{
		ofBitmapFont bitmapFont;
		yVal = ofToString(-getYOffset() / getYScale());
        yValBox = bitmapFont.getBoundingBox(yVal,0,0);
        ofDrawBitmapString(yVal, yValX - yValBox.getRight(), yValY + yValBox.getHeight() / 2);
        
        yVal = ofToString((-getYOffset() + ofGetWindowHeight() / 2) / getYScale());
        yValBox = bitmapFont.getBoundingBox(yVal,0,0);
        ofDrawBitmapString(yVal, yValX - yValBox.getRight(), _min.y - yValBox.getTop() + yLabelPadding);
        
        yVal = ofToString((-getYOffset() - ofGetWindowHeight() / 2) / getYScale());
        yValBox = bitmapFont.getBoundingBox(yVal,0,0);
        ofDrawBitmapString(yVal,  yValX - yValBox.getRight(), _max.y - yLabelPadding);
	}



	for (int i=0; i<_scopePlot.getNumVariables(); i++) {
		// Legend Text
		if (_legendWidth >= 0)
		{
			ofSetColor(_scopePlot.getVariableColor(i));
			string legendString = getVariableName(i);
			float legendX = _min.x + _legendPadding;
			float legendY = _min.y + _legendPadding + _textSpacer*(i + 1);
			if (_legendFont.isLoaded())
			{
				_legendFont.drawString(legendString, legendX, legendY);
			}
			else
			{
				ofDrawBitmapString(legendString, legendX, legendY);
			}
		}
	}	

	// Sets the zero line width when called before plot()
	ofSetLineWidth(_outlineWidth);

	// Plot the Data
	_scopePlot.plot();

	ofSetColor(_outlineColor);
	ofSetLineWidth(_outlineWidth);

	ofEnableAlphaBlending();

	// Legend outline
	if (_legendWidth >= 0)
	{
		ofLine(_min.x, _min.y, _min.x, _max.y);
		ofLine(_min.x, _max.y, _min.x + _legendWidth, _max.y);
		ofLine(_min.x + _legendWidth, _max.y, _min.x + _legendWidth, _min.y);
		ofLine(_min.x + _legendWidth, _min.y, _min.x, _min.y);
	}

	// Scope outline
	//ofLine(_min.x, _min.y, _min.x, _max.y);
	ofLine(_min.x + _legendWidth,	_max.y, _max.x,					_max.y);
	ofLine(_max.x,					_max.y,	_max.x,					_min.y);
	ofLine(_max.x,					_min.y, _min.x + _legendWidth,	_min.y);

	ofDisableAlphaBlending(); 

	ofPopStyle();
}

/*
** getVariableName 
*/
string ofxOscilloscope::getVariableName(int i) {
	if (i < _variableNames.size()) {
		return _variableNames.at(i);
	} else {
		fprintf(stderr, "ERROR - getVariableName: uninitialized or invalid index");
		return NULL;
	}
}


/*-------------------------------------------------
* Multiscope
* Class to create multiple oscilloscope panels
*-------------------------------------------------*/

/*
** ofxMultiScope 
*/
ofxMultiScope::ofxMultiScope() {
}

/*
** ofxMultiScope 
*/
ofxMultiScope::ofxMultiScope(int numScopes, ofRectangle scopeArea, ofTrueTypeFont legendFont, 
	int legendWidth, ofColor outlineColor, ofColor zeroLineColor, ofColor backgroundColor) {
		_min = scopeArea.getTopLeft();
		_max = scopeArea.getBottomRight();
		_numScopes = numScopes;
		scopes.resize(_numScopes);
		for (int i=0; i<_numScopes; i++) {
			ofPoint sMin(_min.x, _min.y + i*(_max.y - _min.y)/_numScopes);
			ofPoint sMax(_max.x, _min.y + (i+1)*(_max.y - _min.y)/_numScopes);
			scopes.at(i) = ofxOscilloscope(sMin, sMax, legendFont, legendWidth, 
				outlineColor, zeroLineColor);
		}
}

/*
** ofxMultiScope 
*/
ofxMultiScope::ofxMultiScope(int numScopes, ofPoint min, ofPoint max, ofTrueTypeFont legendFont, 
	int legendWidth, ofColor outlineColor, ofColor zeroLineColor, ofColor backgroundColor) {
		_min = min;
		_max = max;
		_numScopes = numScopes;
		scopes.resize(_numScopes);
		for (int i=0; i<_numScopes; i++) {
			ofPoint sMin(_min.x, _min.y + i*(_max.y - _min.y)/_numScopes);
			ofPoint sMax(_max.x, _min.y + (i+1)*(_max.y - _min.y)/_numScopes);
			scopes.at(i) = ofxOscilloscope(sMin, sMax, legendFont, legendWidth, 
				outlineColor, zeroLineColor);
		}
}

/*
** ~ofxMultiScope 
*/
ofxMultiScope::~ofxMultiScope() {
}

/*
** TODO setTimeWindow 
*/
void ofxMultiScope::setTimeWindow(float timeWindow) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setTimeWindow(timeWindow);
	}
}

/*
** setPlotLineWidth
** Sets the line width of the plotted data
*/
void ofxMultiScope::setPlotLineWidth(float plotLineWidth) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setPlotLineWidth(plotLineWidth);
	}
}

/*
** setOutlineWidth
** Sets the line width of the scope outlines
*/
void ofxMultiScope::setOutlineWidth(float outlineWidth) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setOutlineWidth(outlineWidth);
	}
}

/*
** setOutlineColor
** Sets the color of the scope outlines
*/
void ofxMultiScope::setOutlineColor(ofColor outlineColor) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setOutlineColor(outlineColor);
	}
}

/*
** setZeroLineColor
** Sets the color of the scope zero line
*/
void ofxMultiScope::setZeroLineColor(ofColor zeroLineColor) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setZeroLineColor(zeroLineColor);
	}
}

/*
** setBackgroundColor
** Sets the color of the background.
*/
void ofxMultiScope::setBackgroundColor(ofColor backgroundColor) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setBackgroundColor(backgroundColor);
	}
}

/*
** setLegendFont
** Sets the font used for the legend text
*/ 
void ofxMultiScope::setLegendFont(ofTrueTypeFont legendFont) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setLegendFont(legendFont);
	}
}

/*
** setAxesFont
** Sets the font used for the legend text
*/
void ofxMultiScope::setAxesFont(ofTrueTypeFont font) {
	for (int i = 0; i < _numScopes; i++) {
		scopes.at(i).setAxesFont(font);
	}
}


/*
** setLegendWidth
** Sets the width of the legend
*/ 
void ofxMultiScope::setLegendWidth(int legendWidth) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setLegendWidth(legendWidth);
	}
}

/*
** setTextSpacing
** Sets the spacing of text in the legend.
** legendPadding is the padding between the legend borders and the text.
** textSpacing the spacing between the text of the variable names.
*/ 
void ofxMultiScope::setTextSpacing(int legendPadding, int textSpacing) {
	for (int i=0; i<_numScopes; i++) {
		scopes.at(i).setTextSpacing(legendPadding, textSpacing);
	}
}

/*
** setPosition
** Sets the position of the multiScope window.
*/ 
void ofxMultiScope::setPosition(ofPoint min, ofPoint max) {
	_min = min;
	_max = max;
	for (int i=0; i<_numScopes; i++) {
		ofPoint sMin(min.x, min.y + i*(max.y - min.y)/_numScopes);
		ofPoint sMax(max.x, min.y + (i+1)*(max.y - min.y)/_numScopes);
		scopes.at(i).setPosition(sMin, sMax);
	}
}
void ofxMultiScope::setPosition(ofRectangle scopeArea) {
	setPosition(scopeArea.getTopLeft(), scopeArea.getBottomRight());
}

/*
** getPosition
** gets the rectangle defining the position of the oscilloscope window.
*/ 
ofRectangle ofxMultiScope::getPosition() {
	ofRectangle rect = ofRectangle((const ofPoint) _min, (const ofPoint) _max);
	return rect;
}


/*
** incrementYScale
** Increases the yScale to yScale*2 of all oscilloscope panels
*/ 
vector<float> ofxMultiScope::incrementYScale() {
	vector<float> ret;
	ret.resize(_numScopes);
	for (int i=0; i<_numScopes; i++) {
		ret.at(i) = scopes.at(i).incrementYScale();
	}
	return ret;
}

/*
** decrementYScale
** Decreases the yScale to yScale/2 of all oscilloscope panels
*/ 
vector<float> ofxMultiScope::decrementYScale() {
	vector<float> ret;
	ret.resize(_numScopes);
	for (int i=0; i<_numScopes; i++) {
		ret.at(i) = scopes.at(i).decrementYScale();
	}
	return ret;
}

/*
** incrementYOffset
** Increases the yOffset++ of all oscilloscope panels
*/ 
vector<float> ofxMultiScope::incrementYOffset() {
	vector<float> ret;
	ret.resize(_numScopes);
	for (int i=0; i<_numScopes; i++) {
		ret.at(i) = scopes.at(i).incrementYOffset();
	}
	return ret;
}

/*
** decrementYOffset
** Decreases the yOffset-- of all oscilloscope panels
*/ 
vector<float> ofxMultiScope::decrementYOffset() {
	vector<float> ret;
	ret.resize(_numScopes);
	for (int i=0; i<_numScopes; i++) {
		ret.at(i) = scopes.at(i).decrementYOffset();
	}
	return ret;
}

/*
** incrementTimeWindow
** Increases the timeWindow to timeWindow*2 of all oscilloscope panels
*/ 
vector<float> ofxMultiScope::incrementTimeWindow() {
	vector<float> ret;
	ret.resize(_numScopes);
	for (int i=0; i<_numScopes; i++) {
		ret.at(i) = scopes.at(i).incrementTimeWindow();
	}
	return ret;
}

/*
** decrementTimeWindow
** Decreases the timeWindow to timeWindow/2 of all oscilloscope panels
*/ 
vector<float> ofxMultiScope::decrementTimeWindow() {
	vector<float> ret;
	ret.resize(_numScopes);
	for (int i=0; i<_numScopes; i++) {
		ret.at(i) = scopes.at(i).decrementTimeWindow();
	}
	return ret;
}


/*
** plot
** Plots the data in all the oscilloscope buffer
*/
void ofxMultiScope::plot() {
	for (int i=0; i<scopes.size(); i++) {
		scopes.at(i).plot();
	}
}

void ofxMultiScope::clearData() {
	for (int i = 0; i<scopes.size(); i++) {
		scopes.at(i).clearData();
	}
}


bool ofxMultiScope::saveScopeSettings(vector<ofxMultiScope> &multiScopes, string filename)
{
	int plotId = 0;

	// ToDo: add remaining parameters that can be specified in ofxOscilloscope

	ofxXmlSettings scopeSettings;
	scopeSettings.clear();

	int nMultiScopes = multiScopes.size();
	for (int m = 0; m < nMultiScopes; m++)
	{
		//scopeSettings.addTag("multiScopes");
		//scopeSettings.pushTag("multiScopes");
		scopeSettings.addTag("multiScope");
		scopeSettings.pushTag("multiScope", m);
		scopeSettings.addValue("x", multiScopes.at(m).getPosition().getX());
		scopeSettings.addValue("y", multiScopes.at(m).getPosition().getY());
		scopeSettings.addValue("width", multiScopes.at(m).getPosition().getWidth());
		scopeSettings.addValue("height", multiScopes.at(m).getPosition().getHeight());
		//scopeSettings.addValue("legendFontName", "verdana.ttf");
		//scopeSettings.addValue("legendFontSize", 12);
		//scopeSettings.addTag("scopes");
		//scopeSettings.pushTag("scopes");
		int nScopes = multiScopes.at(m).scopes.size();
		for (int s = 0; s < nScopes; s++) {
			scopeSettings.addTag("scope");
			scopeSettings.pushTag("scope", s);
			scopeSettings.addValue("samplingFrequency", multiScopes.at(m).scopes.at(s).getSamplingFrequency());
			scopeSettings.addValue("timeWindow", multiScopes.at(m).scopes.at(s).getTimeWindow());
			scopeSettings.addValue("yMin", multiScopes.at(m).scopes.at(s).getYLims().first);
			scopeSettings.addValue("yMax", multiScopes.at(m).scopes.at(s).getYLims().second);
			scopeSettings.addValue("minYSpan", multiScopes.at(m).scopes.at(s).getMinYSpan());
			int nPlots = multiScopes.at(m).scopes.at(s)._scopePlot.getNumVariables();
			for (int p = 0; p < nPlots; p++) {
				scopeSettings.addTag("plot");
				scopeSettings.pushTag("plot", p);
				scopeSettings.addValue("plotId", plotId++);
				scopeSettings.addValue("plotName", multiScopes.at(m).scopes.at(s).getVariableName(p));
				scopeSettings.addTag("plotColor");
				scopeSettings.pushTag("plotColor");
				ofColor plotColor = multiScopes.at(m).scopes.at(s)._scopePlot.getVariableColor(p);
				scopeSettings.addValue("r", plotColor.r);
				scopeSettings.addValue("g", plotColor.g);
				scopeSettings.addValue("b", plotColor.b);
				scopeSettings.popTag(); // pop plotColor
				scopeSettings.popTag(); // pop plot
			}
			scopeSettings.popTag(); // pop scope
		}
		scopeSettings.popTag(); // pop multiScope
	}

	scopeSettings.saveFile(filename);

	return true;
}

vector<ofxMultiScope> ofxMultiScope::loadScopeSettings(string filename)
{
	// ToDo: add remaining parameters that can be specified in ofxOscilloscope

	vector<ofxMultiScope> multiScopes;
	int plotId = 0; // unique ID for each plot

	ofxXmlSettings scopeSettings;
	scopeSettings.loadFile(filename);

	int nMultiScopes = scopeSettings.getNumTags("multiScope");
	for (int m = 0; m < nMultiScopes; m++)
	{
		scopeSettings.pushTag("multiScope", m);
		float x = scopeSettings.getValue("x", 0.f);
		float y = scopeSettings.getValue("y", 0.f);
		float width = scopeSettings.getValue("width", ofGetWindowWidth());
		float height = scopeSettings.getValue("height", ofGetWindowHeight());
		string legendFontName = scopeSettings.getValue("legendFontName", "verdana.ttf");
		int legendFontSize = scopeSettings.getValue("legendFontSize", 12);
		string axesFontName = scopeSettings.getValue("axesFontName", "verdana.ttf");
		int axesFontSize = scopeSettings.getValue("axesFontSize", 10);
		int plotLineWidth = scopeSettings.getValue("plotLineWidth", 3);
		ofRectangle scopeArea = ofRectangle(x, y, width, height);

		int nScopes = scopeSettings.getNumTags("scope");
		ofTrueTypeFont legendFont;
		legendFont.load(legendFontName.c_str(), legendFontSize, true, true);
		multiScopes.emplace_back(nScopes, scopeArea, legendFont); // Setup the multiScope panel
		ofTrueTypeFont axesFont;
		axesFont.load(axesFontName, axesFontSize, true, true);
		multiScopes.at(m).setAxesFont(axesFont);
		multiScopes.at(m).setPlotLineWidth(plotLineWidth);

		for (int s = 0; s < nScopes; s++) {
			// Setup the oscilloscopes

			scopeSettings.pushTag("scope", s);

			float timeWindow = scopeSettings.getValue("timeWindow", 15.f);
			float samplingFrequency = scopeSettings.getValue("samplingFrequency", 15.f);
			float yMin = scopeSettings.getValue("yMin", 0.f);
			float yMax = scopeSettings.getValue("yMax", 0.f);
			float minYSpan = scopeSettings.getValue("minYSpan", 0.f);

			vector<int> plotIds;
			vector<string> plotNames;
			vector<ofColor> plotColors;

			int nPlots = scopeSettings.getNumTags("plot");
			for (int p = 0; p < nPlots; p++) {
				scopeSettings.pushTag("plot", p);
				plotNames.push_back(scopeSettings.getValue("plotName", ofToString(plotId)));
				plotIds.push_back(scopeSettings.getValue("plotId", plotId++));
				scopeSettings.pushTag("plotColor");
				plotColors.push_back(ofColor(
					scopeSettings.getValue("r", 255),
					scopeSettings.getValue("g", 255),
					scopeSettings.getValue("b", 255)
				));
				scopeSettings.popTag(); // plotColor
				scopeSettings.popTag(); // plot p
			}

			multiScopes.at(m).scopes.at(s).setup(timeWindow, samplingFrequency, plotNames, plotColors); // Setup each oscilloscope panel
			if (yMin == yMax) {
				multiScopes.at(m).scopes.at(s).autoscaleY(true, minYSpan);
			}
			else {
				multiScopes.at(m).scopes.at(s).setYLims(pair<float, float>(yMin, yMax));
			}
			scopeSettings.popTag(); // scope s
		}

		scopeSettings.popTag(); // multiScope m
	}
	
	if (plotId == 0) {
		ofLogNotice() << "FILE: " + filename + "NOT FOUND" << endl;
	}
	return multiScopes;
}

vector<vector<vector<int>>> ofxMultiScope::getPlotIds(string filename)
{

	vector<vector<vector<int>>> plotIds;
	int plotId = 0;

	ofxXmlSettings scopeSettings;
	scopeSettings.loadFile(filename);

	int nMultiScopes = scopeSettings.getNumTags("multiScope");
	plotIds.resize(nMultiScopes);
	for (int m = 0; m < nMultiScopes; m++)
	{
		scopeSettings.pushTag("multiScope", m);
		int nScopes = scopeSettings.getNumTags("scope");
		plotIds.at(m).resize(nScopes);
		for (int s = 0; s < nScopes; s++) {
			scopeSettings.pushTag("scope", s);
			int nPlots = scopeSettings.getNumTags("plot");
			plotIds.at(m).at(s).resize(nPlots);
			for (int p = 0; p < nPlots; p++) {
				scopeSettings.pushTag("plot", p);
				string plotName = scopeSettings.getValue("plotName", "");
				int tempPlotId = scopeSettings.getValue("plotId", plotId++);
				plotIds.at(m).at(s).at(p) = tempPlotId;
				scopeSettings.popTag(); // plot p
			}
			scopeSettings.popTag(); // scope s
		}
		scopeSettings.popTag(); // multiScope m
	}

	if (plotId == 0) {
		ofLogNotice() << "FILE: " + filename + "NOT FOUND" << endl;
	}
	return plotIds;
}

unordered_map<int, vector<size_t>> ofxMultiScope::getPlotIdIndexes(string filename)
{
	vector<vector<vector<int>>> plotIds = getPlotIds(filename);
	unordered_map<int, vector<size_t>> plotIdIndexes;
	for (int m = 0; m < plotIds.size(); m++)
	{
		for (int s = 0; s < plotIds.at(m).size(); s++)
		{
			for (int p = 0; p < plotIds.at(m).at(s).size(); p++) {
				vector<size_t> indexes;
				indexes.push_back(m);
				indexes.push_back(s);
				indexes.push_back(p);
				auto result = plotIdIndexes.emplace(plotIds.at(m).at(s).at(p), indexes);
				if (!result.second)
				{
					ofLogWarning() << "Duplicate plot ID found: " << plotIds.at(m).at(s).at(p) << endl;
				}
			}
		}
	}
	return plotIdIndexes;
}
