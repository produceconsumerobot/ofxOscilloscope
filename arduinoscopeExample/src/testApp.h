#pragma once

//
//  arduinoscope
//
//  openFrameworks program to display arduino data as on an oscilloscope
//  
//  Requires:
//	ofxOscilloscope addon
//	StandardFirmata Example running on the Arduino
//
//  Created by Sean Montgomery on 3/31/13.
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//

#include "ofMain.h"
#include "ofxOscilloscope.h"

class testApp : public ofBaseApp{
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

		ofxMultiScope scopeWin;
		int nScopes;
		std::vector<std::vector<float> > data;

		ofArduino arduino;
		int nPlotsScope0;
		int nPlotsScope1;
		float samplingFreq;
		int selectedScope;
};
