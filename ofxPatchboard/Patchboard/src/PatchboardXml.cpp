#include "PatchboardXml.h"

PatchboardBase::ReturnCode PatchboardXml::loadFile(const std::string &patchboardFile)
{
	string _filename = patchboardFile; // ToDo: Consider if _filename should be class member

	string tag;
	string val;
	//string indent = " ";

	if (patchboard.loadFile(_filename))
	{
		patchcords.clear();

		tag = "patchboard";
		if (!patchboard.pushTag(tag))
		{
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}

		tag = "settings";
		if (!patchboard.pushTag(tag))
		{
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}

		tag = "input";
		if (!patchboard.pushTag(tag))
		{
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}

		tag = "type";
		val = patchboard.getValue(tag, "");
		if (val == "")
		{
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}
		inputType = val;
		
		patchboard.popTag(); // pop input

		tag = "output";
		if (!patchboard.pushTag(tag))
		{
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}

		tag = "type";
		val = patchboard.getValue(tag, "");
		if (val == "")
		{
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}
		inputType = val;

		patchboard.popTag(); // pop output
		patchboard.popTag(); // pop settings

		tag = "patchcords";
		if (!patchboard.pushTag(tag))
		{
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}

		int nPatches = patchboard.getNumTags("patch");
		for (int p = 0; p < nPatches; p++)
		{
			tag = "patch";
			patchboard.pushTag(tag, p);
			//cout << "<patch>" << endl;

			tag = "input";
			val = patchboard.getValue(tag, "");
			if (val == "")
			{
				_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
				return ReturnCode::ERR_TAG_NOT_FOUND;
			}
			string key = val;

			tag = "output";
			val = patchboard.getValue(tag, "");
			if (val == "")
			{
				_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
				return ReturnCode::ERR_TAG_NOT_FOUND;
			}
			string value = val;

			vector<string> values = patchcords[key];
			values.push_back(value);
			//cout << indent << key << ":" << value << endl;
			patchcords[key] = values;
			patchboard.popTag(); // pop patch
		}
		patchboard.popTag(); // pop patchcords
		patchboard.popTag(); // pop patchboard

		if (nPatches == 0) {
			tag = "patch";
			_lastErrMsg = MSG_TAG_NOT_FOUND + tag;
			return ReturnCode::ERR_TAG_NOT_FOUND;
		}

		ReturnCode::SUCCESS;
	}
	else
	{
		_lastErrMsg  = MSG_FILE_NOT_FOUND + _filename;
	}
	return ReturnCode::ERR_FILE_NOT_FOUND;
}

PatchboardXml& PatchboardXml::operator=(const PatchboardXml & p)
{
	PatchboardBase::operator=(p);
	patchboard = p.patchboard;

	return *this;
}
