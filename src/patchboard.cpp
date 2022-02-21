#include "patchboard.h"

bool Patchboard::loadFile(const string& patchboardFile)
{
	_filename = patchboardFile;
	ofxXmlSettingsStoredHandle xmlSettings;
	string indent = " ";
	if (xmlSettings.loadFile(_filename))
	{
		settings.input.clear();
		settings.output.clear();
		patchcords.clear();

		xmlSettings.pushTag("patchboard");
		if (xmlSettings.tagExists("settings"))
		{
			xmlSettings.pushTag("settings");
			if (xmlSettings.tagExists("input"))
			{
				xmlSettings.pushTag("input");
				int count = 0;
				cout << "<" << xmlSettings.storedHandle.ToElement()->Value() << ">" << endl;
				for (auto child = xmlSettings.storedHandle.ToNode()->FirstChild(); child; child = child->NextSibling())
				{
					count++;
					cout << indent << child->ToElement()->Value() << ": " << child->ToElement()->GetText() << endl;
					settings.input[child->ToElement()->Value()] = child->ToElement()->GetText();
				}
				if (count == 0)
				{
					cout << "Patchboard::loadFile -- no inputs" << endl;
					return false; // no inputs
				}
				xmlSettings.popTag(); // pop input
			}
			if (xmlSettings.tagExists("output"))
			{
				xmlSettings.pushTag("output");
				int count = 0;
				cout << "<" << xmlSettings.storedHandle.ToElement()->Value() << ">" << endl;
				for (auto child = xmlSettings.storedHandle.ToNode()->FirstChild(); child; child = child->NextSibling())
				{
					count++;
					cout << indent << child->ToElement()->Value() << ": " << child->ToElement()->GetText() << endl;
					settings.output[child->ToElement()->Value()] = child->ToElement()->GetText();
				}
				if (count == 0)
				{
					cout << "Patchboard::loadFile -- no outputs" << endl;
					return false; // no outputs
				}
				xmlSettings.popTag(); // pop output
				xmlSettings.popTag(); // pop settings
			}
		}
		if (xmlSettings.tagExists("patchcords"))
		{
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
	}
	else
	{
		cout << "Patchboard::loadFile: File not found " << _filename << endl;
	}
	return false;
}
