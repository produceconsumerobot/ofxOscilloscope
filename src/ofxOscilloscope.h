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

/*
class ofxScopeData {
private:
	float ** data;
	//float data[10][10];
	int _nData;
	int _nVar;
public:
	ofxScopeData();
	ofxScopeData(int nData, int nVar);
	~ofxScopeData();
	//float& operator[][](int x,int y);
	//float& operator[](int dataInd);
	//float** operator()();
	float& operator()(int dataInd, int varInd) {
		return data[dataInd][varInd];
	}
	//float** operator()() {return data;}
/*
	float& at(int row, int col) {return data[row][col];}
	float** operator()() {return data;}

	class R
    {
         private:
             friend class ofxScopeData; // Only ofxScopeData can create these objects.
             R(ofxScopeData& parent, int row): m_parent(parent), m_row(row) {}
         public:
              float& operator[](int col) {return m_parent.at(m_row, col);}
         private:
              ofxScopeData&  m_parent;
              int m_row;
    };

    R operator[](int row) {return R(*this,row);}

};
*/


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
	//void updateData(ofxScopeData data, int nPoints); // data[_nVariables][nPoints]
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

/*
class ofxScopeArray {
private:
	friend class ofxScopeData;
	int _nVar;
	float * data;
	ofxScopeData&  d_parent;
	int _d_row;

	ofxScopeArray(ofxScopeData& parent, int v_col): d_parent(parent), _d_row(v_col) {}

public:
	ofxScopeArray();
	ofxScopeArray(int nData, int nVar);
	~ofxScopeArray();
	//float& operator[][](int x,int y);
	float& operator[](int varInd);
	//float** operator()();
};


class ofxScopeData {
private:
	ofxScopeArray scopeArray;
	int _nData;

public:
	ofxScopeData();
	ofxScopeData(int nData, int nVar);
	~ofxScopeData();
	//float& operator[][](int x,int y);
	float& operator[](int dataInd);
	//float** operator()();

};
*/

/*
class ofxScopeData {
private:
	float ** data;
	int _nData;
	int _nVar;
public:
	ofxScopeData();
	ofxScopeData(int nData, int nVar);
	~ofxScopeData();
	//float& operator[][](int x,int y);
	float& operator[](int dataInd);
	//float** operator()();

};
*/


