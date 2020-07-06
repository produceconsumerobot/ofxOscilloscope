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

	//static string getAddress(const string& , const string& separator=":"); // address[separator]index
	//static string getIndex(const string&, const string& separator = ":"); // address[separator]index

};




