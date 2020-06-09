#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "ofxXmlSettings.h"

class ofxXmlSettingsStoredHandle : ofxXmlSettings
{
	friend class Patchboard;
};

class Patchboard
{

public:
	string _filename;

	struct Settings
	{
		unordered_map<string, string> input; // <name, value>
		unordered_map<string, string> output; // <name, value>
	} settings;

	unordered_map<string, vector<string>> patchcords; // <string input, vector<string> outputs>

	bool loadFile(const string& patchboardFile = "patchboard.xml");
};


bool Patchboard::loadFile(const string& patchboardFile)
{
	_filename = patchboardFile;
	ofxXmlSettingsStoredHandle xmlSettings;
	string indent = " ";
	if (xmlSettings.loadFile(_filename))
	{
		xmlSettings.pushTag("patchboard");
		xmlSettings.pushTag("settings");

		xmlSettings.pushTag("input");
		int count = 0;
		cout << "<" << xmlSettings.storedHandle.ToElement()->Value() << ">" << endl;
		for (auto child = xmlSettings.storedHandle.ToNode()->FirstChild(); child; child = child->NextSibling())
		{
			count++;
			cout << indent << child->ToElement()->Value() << ": " << child->ToElement()->GetText() << endl;
			settings.input[child->ToElement()->Value()] = child->ToElement()->GetText();
		}
		if (count == 0) return false; // no inputs
		xmlSettings.popTag(); // pop input

		xmlSettings.pushTag("output");
		count = 0;
		cout << "<" << xmlSettings.storedHandle.ToElement()->Value() << ">" << endl;
		for (auto child = xmlSettings.storedHandle.ToNode()->FirstChild(); child; child = child->NextSibling())
		{
			count++;
			cout << indent << child->ToElement()->Value() << ": " << child->ToElement()->GetText() << endl;
			settings.output[child->ToElement()->Value()] = child->ToElement()->GetText();
		}
		if (count == 0) return false; // no inputs
		xmlSettings.popTag(); // pop output
		xmlSettings.popTag(); // pop settings

		xmlSettings.pushTag("patchcords");
		int nPatches = xmlSettings.getNumTags("patch");
		for (int p = 0; p < nPatches; p++) 
		{
			xmlSettings.pushTag("patch", p);
			cout << "<patch>" << endl;
			string key = xmlSettings.getValue("input", "");
			string value = xmlSettings.getValue("output", "");
			vector<string> values = patchcords[key];
			values.push_back(value);
			cout << indent << key << ":" << value << endl;
			patchcords[key] = values;
			xmlSettings.popTag(); // pop patch
		}
		xmlSettings.popTag(); // pop patchcords
		xmlSettings.popTag(); // pop patchboard
		if (nPatches == 0) return false; // no patches

		return true;
	}
	return false;
}

