#pragma once

#include <string>
#include <vector>
#include "ofxXmlSettings.h"
#include "PatchboardBase.h"

//!
//! Patchboard derivative class for parsing XML into a patchboard
//! with a patchcords map.
//!
//! Example:
//!  <patchboard>
//!    <settings>
//!      <input>
//!        <type>EmotiBit::TypeTag</type>
//!      </input>
//!      <output>
//!        <type>ofxOscilloscope</type>
//!      </output>
//!    </settings>
//!    <patchcords>
//!      <patch>
//!        <input>PR</input>
//!        <output>0</output>
//!      </patch>
 //!     <patch>
//!        <input>PR</input>
//!        <output>2</output>
//!      </patch>
//!      <patch>
//!        <input>PI</input>
//!        <output>1</output>
//!      </patch>
//!    </patchcords>
//!  </patchboard>
//!

class ofxXmlSettingsStoredHandle : public ofxXmlSettings
{
	friend class PatchboardXml;
};

class PatchboardXml : public PatchboardBase
{

public:
  ofxXmlSettingsStoredHandle patchboard;
  
	//! Loads and parses a file in XML format to create a patchboard
	//! @param patchboardFile file path to XML patchboard file
	//! @return PatchboardBase::ReturnCode SUCCESS==0
  ReturnCode loadFile(const std::string &patchboardFile = "patchboard.xml");

	PatchboardXml& operator=(const PatchboardXml & p);
};




