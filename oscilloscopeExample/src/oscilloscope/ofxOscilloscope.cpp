//
//  oscilloscope.cpp
//
//  Created by Sean Montgomery on 12/21/12.
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//

#include "oscilloscope.h"

/*-------------------------------------------------
* ofxScopePlot
* Class to handle the plotting of data for ofxOscilloscope
*-------------------------------------------------*/

ofxScopePlot::ofxScopePlot() {
	_variableColors = NULL;
	_buffer = NULL;
}
ofxScopePlot::~ofxScopePlot() {
	// delete _variableColors
	if (_variableColors != NULL) 
		//delete _variableColors;

	// delete _buffer
	if (_buffer != NULL) {
		for (int i=0; i<_nVariables; i++) {
			delete _buffer[i];
		}
		delete _buffer;
	}
}

void ofxScopePlot::setup(float timeWindow, int sampFreq, ofColor variableColors[], int nVariables, ofPoint min, ofPoint max, float yScale, float yOffset) {
	_min = min;
	_max = max;
	_timeWindow = timeWindow;
	_pointsPerWin = floor(_timeWindow * sampFreq); //(sec)
	_sampFreq = sampFreq;
	_nVariables = nVariables;
	_variableColors = new ofColor[_nVariables];
	//_variableColors = variableColors;
	setVariableColors(variableColors, _nVariables);
	_buffer = new float*[_nVariables];
	for (int i=0; i<_nVariables; i++) {
		_buffer[i] = new float[_pointsPerWin];
		for (int j=0; j<_pointsPerWin; j++) {
			_buffer[i][j] = 0.;
		}
	}
	_yScale = yScale;
	_yOffset = yOffset;
}
// updateData
// data[_nVariables][nPoints]
void ofxScopePlot::updateData(float ** data, int nPoints) {
	for (int i=0; i<_nVariables; i++) {
		// Shift the data in buffer
		for (int j=(_pointsPerWin-1); j>(nPoints-1); j--) {
			int cpPoint = j-(nPoints);
			_buffer[i][j] = _buffer[i][cpPoint];
			//printf("%i,%i; ",j, cpPoint);
		}
		//printf("\n");
		// Copy new data into the beginning of the buffer
		for (int j=0; j<nPoints; j++) {
			_buffer[i][nPoints-1-j] = data[i][j];
			//printf("%i,",j);
		}
	}
	//printf("\n");

	//const int cpLen = _pointsPerWin - nPoints; // number of points to shift
	//float * tempBuffer;
	//tempBuffer = new float[cpLen]; // temp buffer to hold data
		//copy(_buffer[i] + nPoints, _buffer[i] + _pointsPerWin, tempBuffer); // copy cpLen of data to tempBuffer
		//copy(tempBuffer, tempBuffer + cpLen, _buffer[i]); // copy tempBuffer into _buffer shifted by nPoints
		//copy(data[i], data[i] + nPoints, _buffer[i] + cpLen); // copy new data into end of buffer

		//copy(_buffer[i], _buffer[i] + cpLen, tempBuffer); // copy cpLen of data to tempBuffer
		//copy(tempBuffer, tempBuffer + cpLen, _buffer[i] + nPoints); // copy tempBuffer into _buffer shifted by nPoints
		//copy(data[i], data[i] + nPoints, _buffer[i]); // copy new data into beginning of buffer
	//}
	//delete tempBuffer;
}
void ofxScopePlot::setVariableColors(ofColor colors[], int nColors) {
	if (nColors != _nVariables) {
		printf("ERROR: nColors != _nVariables");
	} else {
		_variableColors = new ofColor[nColors];
		for (int i=0; i<nColors; i++) {
			_variableColors[i] = colors[i];
		}
	}
}
void ofxScopePlot::plot() {
	//float xPlotScale = ofGetWindowSize().x / _pointsPerWin * ofGetWindowSize().x / (_max.x - _min.x);
	float xPlotScale = (_max.x - _min.x) / _pointsPerWin;// * (_max.x - _min.x) / ofGetWindowSize().x;
	float yPlotScale = (_max.y - _min.y) / ofGetWindowSize().y;
	float yPlotOffset = ((_max.y - _min.y) / 2.);
	//printf("xPlotScale: %f, yPlotScale: %f, yPlotOffset: %f\n", xPlotScale, yPlotScale, yPlotOffset);
	//ofScale(1., 1., 1.);

	// Scope zero line
	ofSetColor(240,240,240);
	ofLine(_min.x, _min.y + (_max.y - _min.y)/2, _max.x, _min.y + (_max.y - _min.y)/2);

	for (int i=0; i<_nVariables; i++) {
		ofSetColor(_variableColors[i]);
		ofSetLineWidth(1);
		for (int j=1; j<_pointsPerWin; j++) {
			ofPoint p1 = ofPoint(_max.x-((float)(j-1)*xPlotScale),  _max.y-(yPlotScale*((_buffer[i][j-1] * _yScale + _yOffset)) + yPlotOffset));
			ofPoint p2 = ofPoint(_max.x-((float)(j)*xPlotScale), _max.y-(yPlotScale*((_buffer[i][j] * _yScale + _yOffset)) + yPlotOffset));
			ofLine(p1, p2);
			//printf("[%i, %i]: ofLine([%.1f, %.1f], [%.1f, %.1f]): buffer[%.1f],[%.1f]\n", i, j, p1.x, p1.y, p2.x, p2.y, _buffer[i][j-1], _buffer[i][j]);
			//ofLine(_max.x-(float)(j-1), (_buffer[i][j-1] * _yScale) + yPlotOffset, _max.x-(float)j, (_buffer[i][j] * _yScale) + yPlotOffset);
			//printf("[%i, %i]: ofLine(%f, %f, %f, %f)\n", i, j, _max.x-(float)(j-1), (_buffer[i][j-1] * _yScale) + yPlotOffset, _max.x-(float)j, (_buffer[i][j] * _yScale) + yPlotOffset);
		}
	}
}

int ofxScopePlot::getNumVariables() {
	return _nVariables;
}
ofColor ofxScopePlot::getVariableColor(int i) {
	if ((_variableColors != NULL) && (i < getNumVariables())) {
		return _variableColors[i];
	} else {
		printf("ERROR - getVariableColor: uninitialized array or invalid index");
		return NULL;
	}
}
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
	_variableNames = NULL;

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
ofxOscilloscope::~ofxOscilloscope() {
	if (_variableNames != NULL) {
		//delete _variableNames;
	}
}

void ofxOscilloscope::setup(float timeWindow, int sampFreq, 
	string variableNames[], ofColor variableColors[], int nVariables, 
	float yScale, float yOffset) {
	ofPoint min = _min;
	min.x = min.x + _legendWidth;
	_scope.setup(timeWindow, sampFreq, variableColors, nVariables, 
		min, _max, yScale, yOffset);
	setVariableNames(variableNames, nVariables);
}

void ofxOscilloscope::setVariableNames(string variableNames[], int nVariables) {
	if (nVariables != _scope.getNumVariables()) {
		printf("ERROR: nVariables != _nVariables");
	} else {
		_variableNames = new string[nVariables];
		for (int i=0; i<nVariables; i++) {
			_variableNames[i] = variableNames[i];
		}
	}
}
void ofxOscilloscope::setVariableColors(ofColor colors[], int nColors){
	_scope.setVariableColors(colors, nColors);
}
void ofxOscilloscope::updateData(float ** data, int nPoints) {
	_scope.updateData(data, nPoints);
}
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
string ofxOscilloscope::getVariableName(int i) {
	if ((_variableNames != NULL) && (i < _scope.getNumVariables())) {
		return _variableNames[i];
	} else {
		printf("ERROR - getVariableName: uninitialized array or invalid index");
		return NULL;
	}
}

/*-------------------------------------------------
* Multiscope
* Class to create multiple oscilloscope panels
*-------------------------------------------------*/

ofxMultiScope::ofxMultiScope() {
	scopes = NULL;
}
ofxMultiScope::ofxMultiScope(int numScopes, ofPoint min, ofPoint max, int legendWidth) {
	_min = min;
	_max = max;
	_numScopes = numScopes;
	scopes = new ofxOscilloscope[_numScopes];
	for (int i=0; i<_numScopes; i++) {
		ofPoint sMin(min.x, min.y + i*(max.y - min.y)/_numScopes);
		ofPoint sMax(max.x, min.y + (i+1)*(max.y - min.y)/_numScopes);
		scopes[i] = ofxOscilloscope(sMin, sMax, legendWidth);
	}
}
ofxMultiScope::~ofxMultiScope() {
	if (scopes != NULL) {
		//delete scopes;
	}
}
void ofxMultiScope::setTimeWindow(float timeWindow) {
}
void ofxMultiScope::setSize(ofPoint min, ofPoint max) {
}
void ofxMultiScope::plot() {
	for (int i=0; i<_numScopes; i++) {
		scopes[i].plot();
	}
}

