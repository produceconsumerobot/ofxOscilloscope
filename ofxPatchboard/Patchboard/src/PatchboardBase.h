#pragma once
#include <string>
#include <vector>
#include <unordered_map>

//!
//! PatchboardBase is a base class to create one to many maps
//! conceptually similar to a musical patchboard. Using 
//! derivative classes, input strings may be parsed from 
//! various formats (JSON, XML, others).
//!
 
class PatchboardBase
{

public:

	enum ReturnCode {
		SUCCESS = 0,
		ERR_TAG_NOT_FOUND = -1,
		ERR_FORMAT_INCORRECT = -2,
		ERR_FILE_NOT_FOUND = -3
	};

	const std::string MSG_TAG_NOT_FOUND = "[Patchboard] tag not found: ";
	const std::string MSG_FILE_NOT_FOUND = "[Patchboard] file not found: ";
  
  std::string inputType = "";
	std::string outputType = "";
	std::unordered_map<std::string, std::vector<std::string>> patchcords; // One to many patchcord map

  size_t getNumPatches();
	std::string getLastErrMsg();

	PatchboardBase& operator=(const PatchboardBase & p);

	std::string _lastErrMsg = "";
};




