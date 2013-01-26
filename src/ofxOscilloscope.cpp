//
//  oscilloscope.cpp
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
ofxScopePlot::ofxScopePlot() {
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
** min				Specifies the top-left point of the ScopePlot
** max				Specifies the bottom-right of the ScopePlot
** yScale			Muliplier for Y scaling axis
** yOffset			Offset for the Y axis
*/
void ofxScopePlot::setup(float timeWindow, int sampFreq, std::vector<ofColor> variableColors, 
	ofPoint min, ofPoint max, float yScale, float yOffset) {
		_min = min;
		_max = max;
		_timeWindow = timeWindow;
		_pointsPerWin = floor(_timeWindow * sampFreq); //(sec)
		_sampFreq = sampFreq;
		_nVariables = variableColors.size();

		for (int i=0; i<_nVariables; i++) {
			_buffer.push_back(std::vector<float>(_pointsPerWin, 0.));
		}

		_yScale = yScale;
		_yOffset = yOffset;

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
** min				Specifies the top-left point of the ScopePlot
** max				Specifies the bottom-right of the ScopePlot
** yScale			Muliplier for Y scaling axis
** yOffset			Offset for the Y axis
*/
void ofxScopePlot::setup(float timeWindow, int sampFreq, ofColor variableColors[], int nVariables, 
		ofPoint min, ofPoint max, float yScale, float yOffset) {

			std::vector<ofColor> vec_variableColors;
			vec_variableColors.assign(variableColors, variableColors + nVariables);

			setup(timeWindow, sampFreq, vec_variableColors, min, max, yScale, yOffset);
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
** plot
** Plots the data in the buffer
*/
void ofxScopePlot::plot() {
	//float xPlotScale = ofGetWindowSize().x / _pointsPerWin * ofGetWindowSize().x / (_max.x - _min.x);
	float xPlotScale = (_max.x - _min.x) / (_pointsPerWin - 1);// * (_max.x - _min.x) / ofGetWindowSize().x;
	float yPlotScale = (_max.y - _min.y) / ofGetWindowSize().y;
	float yPlotOffset = ((_max.y - _min.y) / 2.);
	//printf("xPlotScale: %f, yPlotScale: %f, yPlotOffset: %f\n", xPlotScale, yPlotScale, yPlotOffset);
	//ofScale(1., 1., 1.);

	// Scope zero line
	ofSetColor(240,240,240);
	ofLine(_min.x, _min.y + (_max.y - _min.y)/2, _max.x, _min.y + (_max.y - _min.y)/2);

	for (int i=0; i<getNumVariables(); i++) {
		ofSetColor(_variableColors.at(i));
		ofSetLineWidth(1);
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
ofxOscilloscope::ofxOscilloscope(ofPoint min, ofPoint max, int legendWidth) {
	_min = min;
	_max = max;
	_legendWidth = legendWidth;

	_legendFont.loadFont("verdana.ttf", 12, true, true);
		//_legendFont.setLineHeight(18.0f);
	//_legendFont.setLetterSpacing(1.037);
	//ofTrueTypeFont::setGlobalDpi(72);

	_legendPadding = 10;
	_textSpacer = 20;
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
void ofxOscilloscope::setup(float timeWindow, int sampFreq, 
	std::vector<string> variableNames, std::vector<ofColor> variableColors, 
	float yScale, float yOffset) {

		if (variableNames.size() != variableColors.size()) {
			fprintf(stderr, "ERROR: variableNames.size() != variableColors.size()");
		} else {
			ofPoint min = _min;
			min.x = min.x + _legendWidth;
			_scope.setup(timeWindow, sampFreq, variableColors, min, _max, yScale, yOffset);
			setVariableNames(variableNames);
		}
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
void ofxOscilloscope::setup(float timeWindow, int sampFreq, string variableNames[], 
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
	if (variableNames.size() != _scope.getNumVariables()) {
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
	_scope.setVariableColors(colors);
}

/*
** setVariableColors
** Must be called after setup with the same number of variables initialized.
*/
void ofxOscilloscope:: setVariableColors(ofColor colors[], int nColors) {
		std::vector<ofColor> vec_colors;
		vec_colors.assign(colors, colors + nColors);

		setVariableColors(vec_colors);
}

/*
void ofxOscilloscope::updateData(float ** data, int nPoints) {
	_scope.updateData(data, nPoints);
}
*/

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
	_scope.updateData(data);
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
	_scope.updateData(data, nPoints);
}


/*
** plot
** Plots the data in the buffer
*/
void ofxOscilloscope::plot(){
	for (int i=0; i<_scope.getNumVariables(); i++) {

		// Legend
		ofSetColor(_scope.getVariableColor(i));
		_legendFont.drawString(getVariableName(i), 
			_min.x + _legendPadding, _min.y + _legendPadding + _textSpacer*(i+1));

		// Timescale
		ofSetColor(200,200,200);
		_legendFont.drawString(ofToString(_scope.getTimeWindow()) + " sec", 
			_min.x + _legendWidth + _legendPadding, 
			_max.y - _legendPadding);

		// Legend outline
		ofLine(_min.x,					_min.y,	_min.x,					_max.y);
		ofLine(_min.x,					_max.y,	_min.x + _legendWidth,	_max.y);
		ofLine(_min.x + _legendWidth,	_max.y,	_min.x + _legendWidth,	_min.y);
		ofLine(_min.x + _legendWidth,	_min.y,	_min.x,					_min.y);

		// Scope outline
		//ofLine(_min.x, _min.y, _min.x, _max.y);
		ofLine(_min.x + _legendWidth,	_max.y, _max.x,					_max.y);
		ofLine(_max.x,					_max.y,	_max.x,					_min.y);
		ofLine(_max.x + _legendWidth,	_min.y, _min.x + _legendWidth,	_min.y);

		_scope.plot();
	}
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
ofxMultiScope::ofxMultiScope(int numScopes, ofPoint min, ofPoint max, int legendWidth) {
	_min = min;
	_max = max;
	_numScopes = numScopes;
	scopes.resize(_numScopes);
	for (int i=0; i<_numScopes; i++) {
		ofPoint sMin(min.x, min.y + i*(max.y - min.y)/_numScopes);
		ofPoint sMax(max.x, min.y + (i+1)*(max.y - min.y)/_numScopes);
		scopes.at(i) = ofxOscilloscope(sMin, sMax, legendWidth);
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
}

/*
** TODO: setSize 
*/
void ofxMultiScope::setSize(ofPoint min, ofPoint max) {
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
